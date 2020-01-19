#include "l.h"
#include "freertos_obj.h"
#include "freertos_headers.h"

#include <stdlib.h>
#include <errno.h>

#include "lauxlib.h"
#include "lua.h"

#include <string.h>

#include "ff.h"
#include "lua_ext_func.h"


static StaticTask_t lua_main_thread_buffer = {0};


static StackType_t lua_main_thread_stack[LUA_MAIN_THREAD_STACK_SIZE] = {0};

static FRESULT fail_close (FATFS *fat) {
    printf("[SYS] Start f_unmount\r\n");
    FRESULT fr = f_unmount("0:");
    if (fr != FR_OK) {
        printf("[SYS] Fail f_unmount. Code: %u\r\n", fr);
    }
    free(fat);

    return fr;
}

static void lua_message (const char *pname, const char *msg) {
    if (pname) {
        printf("[SYS_LUA_ERR] %s: ", pname);
    }

    printf("%s\r\n", msg);
}

static int lua_report (lua_State *L) {
    const char *msg = lua_tostring(L, -1);
    lua_message("lua", msg);
    lua_pop(L, 1);
}

static int load_start_scripts (lua_State *L) {
    int rv = 0;

    FATFS *fat = malloc(sizeof(FATFS));

    if (fat == NULL) {
        return -1;
    }

    printf("\r");
    printf("[SYS] Start f_mount\r\n");
    FRESULT fr = f_mount(fat, "0:", 1);
    if (fr != FR_OK) {
        printf("[SYS] Fail f_mount. Code: %u\r\n", fr);
        free(fat);
        return -1;
    }

    printf("[SYS] start load lua_scripts\r\n");
    if ((rv = lua_fatfs_loadfilex(L, "lua_scripts/init.lua")) != 0) {
        printf("[SYS] Fail load lua_scripts. Code: %u\r\n", rv);
        fail_close(fat);
        return -1;
    }

    printf("[SYS] start lua_pcall\r\n");
    if ((rv = lua_pcall(L, 0, LUA_MULTRET, 0)) != 0) {
        lua_report(L);
        printf("[SYS] Fail lua_pcall. Code: %u\r\n", rv);
        fail_close(fat);
        return -1;
    }

    if (fail_close(fat) != 0) {
        return -1;
    }

    printf("[SYS] Start luaL_dostring('win:start()')\r\n");
    if ((rv = luaL_dostring(L, "win:start()")) != 0) {
        lua_report(L);
        printf("[SYS] Fail luaL_dostring('win:start()'). Code: %u\r\n", rv);
        return -1;
    }

    return 0;
}

static void lua_main_thread (void *p) {
    lua_State *L = NULL;
    L = luaL_newstate();
    lua_open_aym_libs(L);
    load_start_scripts(L);

    while (1) {
        vTaskDelay(1000);
    }
}

int init_lua_main_thread () {
    xTaskCreateStatic(lua_main_thread, "lua_main",
                      LUA_MAIN_THREAD_STACK_SIZE, NULL, LUA_MAIN_THREAD_PRIO,
                      lua_main_thread_stack, &lua_main_thread_buffer);

    return 0;
}
