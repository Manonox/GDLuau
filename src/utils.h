#pragma once

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>

#include <lua.h>
#include <lualib.h>


void lua_pushvariant(lua_State *L, const godot::Variant &var);
void lua_pusharray(lua_State *L, const godot::Array &array);
void lua_pushdictionary(lua_State *L, const godot::Dictionary &dict);

godot::Variant lua_tovariant(lua_State *L, int idx);
godot::Array lua_toarray(lua_State *L, int idx);
godot::Dictionary lua_todictionary(lua_State *L, int idx);

bool luaL_isarray(lua_State *L, int idx);

void lua_pushcallable(lua_State *L, const godot::Callable &callable);

