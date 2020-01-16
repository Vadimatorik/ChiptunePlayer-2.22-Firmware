#pragma once

#include "lua.h"

LUAMOD_API int luaopen_lcd (lua_State *L);
LUAMOD_API int luaopen_freertos (lua_State *L);
LUAMOD_API int luaopen_fat (lua_State *L);
LUAMOD_API int luaopen_player (lua_State *L);

void lua_open_aym_libs (lua_State *L);

int init_lua_main_thread ();