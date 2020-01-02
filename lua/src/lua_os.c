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

static const luaL_Reg os_lib[] = {
    {"delay_ms", lua_delay_ms},
    {NULL, NULL}
};

LUAMOD_API int luaopen_os (lua_State *L) {
    luaL_newlib(L, os_lib);
    return 1;
}
