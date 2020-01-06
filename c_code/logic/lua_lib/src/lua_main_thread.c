#include "l.h"
#include "freertos_obj.h"
#include "freertos_headers.h"

#include "lauxlib.h"
#include "lua.h"

__attribute__ ((section (".bss_ccm")))
static StaticTask_t lua_main_thread_buffer = {0};

__attribute__ ((section (".bss_ccm")))
static StackType_t lua_main_thread_stack[LUA_MAIN_THREAD_STACK_SIZE] = {0};

static int load_start_scripts (lua_State *L) {
    if (luaL_dostring(L, "_G.lua_scripts_path_dir = '../ChiptunePlayer-2.22-Firmware/lua_scripts/'") != 0) {
        return -1;
    }

    if (luaL_dofile(L, "../ChiptunePlayer-2.22-Firmware/lua_scripts/init.lua")) {
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
