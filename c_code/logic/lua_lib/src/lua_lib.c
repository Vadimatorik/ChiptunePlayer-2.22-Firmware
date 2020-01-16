#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"
#include "l.h"

static const luaL_Reg loaded_aym_libs[] = {
    {"_G",            luaopen_base},
    {LUA_TABLIBNAME,  luaopen_table},
    {LUA_IOLIBNAME,   luaopen_io},
    {LUA_STRLIBNAME,  luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {"lcd",           luaopen_lcd},
    {"os",            luaopen_freertos},
    {"fat",           luaopen_fat},
    {"aym",           luaopen_player},
    {NULL, NULL}
};

void lua_open_aym_libs (lua_State *L) {
    const luaL_Reg *lib;

    for (lib = loaded_aym_libs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);
    }
}
