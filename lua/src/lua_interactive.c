#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"

#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <signal.h>

#include "freertos_headers.h"
#include "mc_hardware.h"

lua_State *L = NULL;

static const char LUA_PROMPT[3] = "> ";
static const char LUA_PROMPT2[4] = ">> ";

static const uint32_t LUA_STRING_MAX_LEN = 1024;

static int add_lua_libs () {
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

static const char *get_prompt (int firstline) {
    const char *p = NULL;

    lua_getglobal(L, firstline?"_PROMPT":"_PROMPT2");

    p = lua_tostring(L, -1);

    if (p == NULL) {
        p = (firstline?LUA_PROMPT:LUA_PROMPT2);
    }

    return p;
}

static void lua_readline (char *b, const char *p) {
    fputs(p, stdout);
    get_string_from_uart(b, LUA_STRING_MAX_LEN);
}

static int pushline (int firstline) {
    char *b = malloc(sizeof(char) * LUA_STRING_MAX_LEN);
    size_t l = 0;
    const char *prmt = get_prompt(firstline);
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

static int addreturn () {
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


static void lua_message (const char *pname, const char *msg) {
    if (pname) lua_writestringerror("%s: ", pname);
    lua_writestringerror("%s\n", msg);
}


static void lua_print () {
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

static int incomplete (int status) {
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

static int multiline () {
    for (;;) {
        size_t len;
        const char *line = lua_tolstring(L, 1, &len);
        int status = luaL_loadbuffer(L, line, len, "=stdin");
        if (!incomplete(status) || !pushline(0)) {
            return status;
        }

        lua_pushliteral(L, "\n");
        lua_insert(L, -2);
        lua_concat(L, 3);
    }
}

static int lua_loadline () {
    int status;
    lua_settop(L, 0);
    if (!pushline(1)) {
        return -1;
    }

    if ((status = addreturn()) != LUA_OK) {
        status = multiline();
    }

    lua_remove(L, 1);
    lua_assert(lua_gettop(L) == 1);
    return status;
}

static int msghandler () {
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

static void lstop (lua_State *L, lua_Debug *ar) {
    ar = ar;
    lua_sethook(L, NULL, 0, 0);
    luaL_error(L, "interrupted!");
}

static void laction (int i) {
    signal(i, SIG_DFL);
    lua_sethook(L, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}

static int lua_docall (int narg, int nres) {
    int status;
    int base = lua_gettop(L) - narg;
    lua_pushcfunction(L, msghandler);
    lua_insert(L, base);
    signal(SIGINT, laction);
    status = lua_pcall(L, narg, nres, base);
    signal(SIGINT, SIG_DFL);
    lua_remove(L, base);
    return status;
}

static int lua_report (int status) {
    if (status != LUA_OK) {
        const char *msg = lua_tostring(L, -1);
        lua_message("lua", msg);
        lua_pop(L, 1);  /* remove message */
    }
    return status;
}

void task_lua_interactive (void *p) {
    p = p;

    L = luaL_newstate();
    if (L == NULL) {
        while (1) {
            vTaskDelay(1000);
        }
    }

    if (add_lua_libs() != 0) {
        while (1) {
            vTaskDelay(1000);
        }
    }

    while (1) {
        int status;
        while ((status = lua_loadline()) != -1) {
            if (status == LUA_OK)
                status = lua_docall(0, LUA_MULTRET);
            if (status == LUA_OK) {
                lua_print();
            } else {
                lua_report(status);
            }
        }
    }
}
