#include "lua.h"

#include "lauxlib.h"

#include "freertos_headers.h"

static int lua_delay_ms (lua_State *L) {
    int stack = 0;

    int delay = luaL_checkinteger(L, ++stack);
    if (delay < 0) {
        luaL_argcheck(L, 0, stack, "invalid value");
    }

    vTaskDelay(delay);

    return 0;
}

void matrix_keyboard_event_press (uint32_t key) {

}

void matrix_keyboard_event_click (uint32_t key) {

}

void matrix_keyboard_event_long_press (uint32_t key) {

}

void matrix_keyboard_event_long_click (uint32_t key) {

}

static const luaL_Reg os_lib[] = {
    {"delay_ms", lua_delay_ms},
    {NULL, NULL}
};

LUAMOD_API int luaopen_os (lua_State *L) {
    luaL_newlib(L, os_lib);
    return 1;
}
