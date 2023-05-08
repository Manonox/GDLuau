#ifndef LUAU_UTILS_H
#define LUAU_UTILS_H

#include <lua.h>
#include <lualib.h>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <classes/luau_function.h>



void lua_pushvariant(lua_State *L, const godot::Variant &var);
void lua_pusharray(lua_State *L, const godot::Array &array);
void lua_pushdictionary(lua_State *L, const godot::Dictionary &dict);

godot::Variant lua_tovariant(lua_State *L, int idx);
godot::Array lua_toarray(lua_State *L, int idx);
godot::Dictionary lua_todictionary(lua_State *L, int idx);

bool luaL_isarray(lua_State *L, int idx);

godot::Ref<godot::LuauFunction> lua_tofunction(lua_State *L, int idx);

#endif
