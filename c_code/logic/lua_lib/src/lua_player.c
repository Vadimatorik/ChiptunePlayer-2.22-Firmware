#include "lua.h"

#include "lauxlib.h"
#include "ff.h"
#include <stdlib.h>

#include "aym_psg_parser.h"

static int lua_get_sec_len (lua_State *L) {
    const char *path = luaL_checkstring(L, 1);

    uint32_t len_sec = 0;
    int rv = aym_psg_get_len_sec(path, &len_sec);

    if (rv < 0) {
        lua_pushnumber(L, -rv);
    } else {
        lua_pushnumber(L, len_sec);
    }

    return 1;
}

static const luaL_Reg player_lib[] = {
    {"get_sec_len", lua_get_sec_len},
    {NULL, NULL}
};

LUAMOD_API int luaopen_player (lua_State *L) {
    luaL_newlib(L, player_lib);
    return 1;
}
