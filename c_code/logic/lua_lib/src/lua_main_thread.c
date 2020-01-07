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

__attribute__ ((section (".bss_ccm")))
static StaticTask_t lua_main_thread_buffer = {0};

__attribute__ ((section (".bss_ccm")))
static StackType_t lua_main_thread_stack[LUA_MAIN_THREAD_STACK_SIZE] = {0};

static int load_start_scripts (lua_State *L) {
    int rv = 0;

    FATFS *fat = malloc(sizeof(FATFS));

    if (fat == NULL) {
        return ENOMEM;
    }

    if (f_mount(fat, "0:", 1) != FR_OK) {
        free(fat);
        return EIO;
    }

    if (lua_fatfs_loadfilex(L, "lua_scripts/init.lua") != 0) {
        f_unmount("0:");
        free(fat);
        return -1;
    }

    if ((rv = lua_pcall(L, 0, LUA_MULTRET, 0)) != 0) {
        f_unmount("0:");
        free(fat);
        return rv;
    }

    f_unmount("0:");
    free(fat);

    if (luaL_dostring(L, "win:start()") != 0) {
        return -1;
    }

    return 0;
}

static void lua_main_thread (void *p) {
    lua_State*L = NULL;
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
