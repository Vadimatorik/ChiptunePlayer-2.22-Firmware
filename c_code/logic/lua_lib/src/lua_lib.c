#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"
#include "l.h"

static const luaL_Reg loaded_aym_libs[] = {
    {LUA_GNAME,       luaopen_base},
    {LUA_TABLIBNAME,  luaopen_table},
    {LUA_STRLIBNAME,  luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {"lcd",           luaopen_lcd},
    {"os",            luaopen_os},
    {"fat",           luaopen_fat},
    {NULL, NULL}
};

void lua_open_aym_libs (lua_State *L) {
    const luaL_Reg *lib;

    for (lib = loaded_aym_libs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);
    }
}
