#include "freertos_headers.h"
#include "hardware.h"

#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"

#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <signal.h>

#define TASK_LUA_STACK_SIZE 1000
StaticTask_t task_lua_buf;
StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];
const char LUA_PROMPT[3] = "> ";
const char LUA_PROMPT2[4] = ">> ";

static const uint32_t LUA_STRING_MAX_LEN = 1024;

int add_lua_libs (lua_State *L) {
    if (luaopen_base(L) != 1) {
        return -1;
    }

    if (luaopen_coroutine(L) != 1) {
        return -1;
    }

    if (luaopen_table(L) != 1) {
        return -1;
    }

    if (luaopen_string(L) != 1) {
        return -1;
    }

    return 0;
}


const char *get_prompt (lua_State *L, int firstline) {
    const char *p = NULL;

    lua_getglobal(L, firstline?"_PROMPT":"_PROMPT2");

    p = lua_tostring(L, -1);

    if (p == NULL) {
        p = (firstline?LUA_PROMPT:LUA_PROMPT2);
    }

    return p;
}

void lua_readline (char *b, const char *p) {
    fputs(p, stdout);
    fgets(b, LUA_STRING_MAX_LEN - 1, stdin);
}

int pushline (lua_State *L, int firstline) {
    char *b = malloc(sizeof(char) * LUA_STRING_MAX_LEN);
    size_t l = 0;
    const char *prmt = get_prompt(L, firstline);
    lua_readline(b, prmt);
    lua_pop(L, 1);
    l = strlen(b);

    if (l > 0 && b[l - 1] == '\n') {
        b[--l] = '\0';
    }

    if (firstline && b[0] == '=') {
        lua_pushfstring(L, "return %s", b + 1);
    } else {
        lua_pushlstring(L, b, l);
    }

    free(b);
    return 1;
}

int addreturn (lua_State *L) {
    const char *line = lua_tostring(L, -1);
    const char *retline = lua_pushfstring(L, "return %s;", line);
    int status = luaL_loadbuffer(L, retline, strlen(retline), "=stdin");
    if (status == LUA_OK) {
        lua_remove(L, -2);
    } else {
        lua_pop(L, 2);
    }

    return status;
}


void lua_message (const char *pname, const char *msg) {
    if (pname) lua_writestringerror("%s: ", pname);
    lua_writestringerror("%s\n", msg);
}


void lua_print (lua_State *L) {
    int n = lua_gettop(L);
    if (n > 0) {  /* any result to be printed? */
        luaL_checkstack(L, LUA_MINSTACK, "too many results to print");
        lua_getglobal(L, "print");
        lua_insert(L, 1);
        if (lua_pcall(L, n, 0, 0) != LUA_OK)
            lua_message("lua", lua_pushfstring(L, "error calling 'print' (%s)",
                                             lua_tostring(L, -1)));
    }
}

#define EOFMARK	"<eof>"
#define marklen	(sizeof(EOFMARK)/sizeof(char) - 1)

int incomplete (lua_State *L, int status) {
    if (status == LUA_ERRSYNTAX) {
        size_t lmsg;
        const char *msg = lua_tolstring(L, -1, &lmsg);
        if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0) {
            lua_pop(L, 1);
            return 1;
        }
    }
    return 0;
}

int multiline (lua_State *L) {
    for (;;) {
        size_t len;
        const char *line = lua_tolstring(L, 1, &len);
        int status = luaL_loadbuffer(L, line, len, "=stdin");
        if (!incomplete(L, status) || !pushline(L, 0)) {
            return status;
        }

        lua_pushliteral(L, "\n");
        lua_insert(L, -2);
        lua_concat(L, 3);
    }
}



int lua_loadline (lua_State *L) {
    int status;
    lua_settop(L, 0);
    if (!pushline(L, 1)) {
        return -1;
    }

    if ((status = addreturn(L)) != LUA_OK) {
        status = multiline(L);
    }

    lua_remove(L, 1);
    lua_assert(lua_gettop(L) == 1);
    return status;
}

int msghandler (lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                                  luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

static lua_State *globalL = NULL;

static void lstop (lua_State *L, lua_Debug *ar) {
    (void)ar;
    lua_sethook(L, NULL, 0, 0);
    luaL_error(L, "interrupted!");
}


static void laction (int i) {
    signal(i, SIG_DFL);
    lua_sethook(globalL, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}

int lua_docall (lua_State *L, int narg, int nres) {
    int status;
    int base = lua_gettop(L) - narg;
    lua_pushcfunction(L, msghandler);
    lua_insert(L, base);
    globalL = L;
    signal(SIGINT, laction);
    status = lua_pcall(L, narg, nres, base);
    signal(SIGINT, SIG_DFL);
    lua_remove(L, base);
    return status;
}


int lua_report (lua_State *L, int status) {
    if (status != LUA_OK) {
        const char *msg = lua_tostring(L, -1);
        lua_message("lua", msg);
        lua_pop(L, 1);  /* remove message */
    }
    return status;
}

void task_lua (void *p) {
    p = p;

    lua_State *l = luaL_newstate();
    if (l == NULL) {
        while (1) {
            vTaskDelay(1000);
        }
    }

    if (add_lua_libs(l) != 0) {
        while (1) {
            vTaskDelay(1000);
        }
    }

    while (1) {
        int status;
        while ((status = lua_loadline(l)) != -1) {
            if (status == LUA_OK)
                status = lua_docall(l, 0, LUA_MULTRET);
            if (status == LUA_OK) {
                lua_print(l);
            } else {
                lua_report(l, status);
            }
        }
    }
}

int main () {
    int rv = 0;

    if ((rv = init_core()) != 0) {
        return rv;
    }

    if ((rv = init_gpio()) != 0) {
        return rv;
    }

    if ((rv = init_rcc()) != 0) {
        return rv;
    }

    if ((rv = init_uart()) != 0) {
        return rv;
    }

    if (xTaskCreateStatic(task_lua, "lua", TASK_LUA_STACK_SIZE, NULL, 3, task_lua_stack, &task_lua_buf) == NULL) {
        return ENOMEM;
    }

    vTaskStartScheduler();
}
