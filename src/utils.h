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
godot::Variant lua_toarray(lua_State *L, int idx);
godot::Variant lua_todictionary(lua_State *L, int idx);

bool luaL_isarray(lua_State *L, int idx);

void lua_pushcallable(lua_State *L, const godot::Callable &callable);


bool luaL_hasmetatable(lua_State *L, int idx, const char *tname);

void lua_pushobject(lua_State *L, godot::Object *object);
void object_userdata_dtor(lua_State *L, void *data);
godot::Object *lua_toobject(lua_State *L, int idx);
int lua_isobject(lua_State *L, int idx);
godot::Object *luaL_checkobject(lua_State *L, int idx, bool valid);
