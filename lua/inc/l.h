#pragma once

#include "lua.h"

LUAMOD_API int luaopen_lcd (lua_State *L);

#define LUA_EXTRALIBS {"lcd", luaopen_lcd},

void task_lua_interactive (void *p);