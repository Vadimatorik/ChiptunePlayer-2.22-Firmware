#pragma once

#include "lua.h"

LUAMOD_API int luaopen_lcd (lua_State *L);
LUAMOD_API int luaopen_os (lua_State *L);
LUAMOD_API int luaopen_fat (lua_State *L);

void lua_open_aym_libs (lua_State *L);

void task_lua_interactive (void *p);