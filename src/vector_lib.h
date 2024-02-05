#pragma once

#include <lua.h>
#include <cstdlib>


#define LUA_VECLIBNAME "vector"


int lua_vector_lib_call(lua_State *L);

int lua_vector_tostring(lua_State *L);

int lua_vector_index(lua_State *L);
int lua_vector_newindex(lua_State *L);

int luaopen_vector(lua_State *L);
