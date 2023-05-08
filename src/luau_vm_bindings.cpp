#include <luau_vm.h>
#include <utils.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;


void LuauVM::_bind_passthrough_methods() {
    ClassDB::bind_method(D_METHOD("luacall", "nargs", "nresults"), &LuauVM::luacall);
    ClassDB::bind_method(D_METHOD("concat", "n"), &LuauVM::concat);
    ClassDB::bind_method(D_METHOD("createtable", "narr", "nrec"), &LuauVM::createtable);
    ClassDB::bind_method(D_METHOD("equal", "index1", "index2"), &LuauVM::equal);
    ClassDB::bind_method(D_METHOD("error"), &LuauVM::error);
    ClassDB::bind_method(D_METHOD("getfenv", "index"), &LuauVM::getfenv);
    ClassDB::bind_method(D_METHOD("getfield", "index", "key"), &LuauVM::getfield);
    ClassDB::bind_method(D_METHOD("getglobal", "key"), &LuauVM::getglobal);
    ClassDB::bind_method(D_METHOD("getmetatable", "index"), &LuauVM::getmetatable);
    ClassDB::bind_method(D_METHOD("gettable", "index"), &LuauVM::gettable);
    ClassDB::bind_method(D_METHOD("gettop"), &LuauVM::gettop);
    ClassDB::bind_method(D_METHOD("insert", "index"), &LuauVM::insert);
    ClassDB::bind_method(D_METHOD("isboolean", "index"), &LuauVM::isboolean);
    ClassDB::bind_method(D_METHOD("isfunction", "index"), &LuauVM::isfunction);
    ClassDB::bind_method(D_METHOD("isnil", "index"), &LuauVM::isnil);
    ClassDB::bind_method(D_METHOD("isnone", "index"), &LuauVM::isnone);
    ClassDB::bind_method(D_METHOD("isnoneornil", "index"), &LuauVM::isnoneornil);
    ClassDB::bind_method(D_METHOD("isnumber", "index"), &LuauVM::isnumber);
    ClassDB::bind_method(D_METHOD("isstring", "index"), &LuauVM::isstring);
    ClassDB::bind_method(D_METHOD("istable", "index"), &LuauVM::istable);
    ClassDB::bind_method(D_METHOD("isthread", "index"), &LuauVM::isthread);
    ClassDB::bind_method(D_METHOD("isuserdata", "index"), &LuauVM::isuserdata);
    ClassDB::bind_method(D_METHOD("lessthan", "index1", "index2"), &LuauVM::lessthan);
    ClassDB::bind_method(D_METHOD("newtable"), &LuauVM::newtable);
    // ClassDB::bind_method(D_METHOD("newuserdata", "size"), &LuauVM::newuserdata);
    ClassDB::bind_method(D_METHOD("next", "index"), &LuauVM::next);
    ClassDB::bind_method(D_METHOD("objlen", "index"), &LuauVM::objlen);
    ClassDB::bind_method(D_METHOD("pcall", "nargs", "errfunc"), &LuauVM::pcall);
    ClassDB::bind_method(D_METHOD("pop", "n"), &LuauVM::pop);
    ClassDB::bind_method(D_METHOD("pushnil"), &LuauVM::pushnil);
    ClassDB::bind_method(D_METHOD("pushboolean", "b"), &LuauVM::pushboolean);
    ClassDB::bind_method(D_METHOD("pushinteger", "x"), &LuauVM::pushinteger);
    ClassDB::bind_method(D_METHOD("pushnumber", "x"), &LuauVM::pushnumber);
    ClassDB::bind_method(D_METHOD("pushstring", "s"), &LuauVM::pushstring);
    ClassDB::bind_method(D_METHOD("pushvalue", "index"), &LuauVM::pushvalue);
    ClassDB::bind_method(D_METHOD("rawequal", "index1", "index2"), &LuauVM::rawequal);
    ClassDB::bind_method(D_METHOD("rawget", "index"), &LuauVM::rawget);
    ClassDB::bind_method(D_METHOD("rawgeti", "index", "n"), &LuauVM::rawgeti);
    ClassDB::bind_method(D_METHOD("rawset", "index"), &LuauVM::rawset);
    ClassDB::bind_method(D_METHOD("rawseti", "index", "n"), &LuauVM::rawseti);
    ClassDB::bind_method(D_METHOD("remove", "index"), &LuauVM::remove);
    ClassDB::bind_method(D_METHOD("replace", "index"), &LuauVM::replace);
    ClassDB::bind_method(D_METHOD("setfenv", "index"), &LuauVM::setfenv);
    ClassDB::bind_method(D_METHOD("setfield", "index", "key"), &LuauVM::setfield);
    ClassDB::bind_method(D_METHOD("setglobal", "key"), &LuauVM::setglobal);
    ClassDB::bind_method(D_METHOD("setmetatable", "index"), &LuauVM::setmetatable);
    ClassDB::bind_method(D_METHOD("settable", "index"), &LuauVM::settable);
    ClassDB::bind_method(D_METHOD("settop", "index"), &LuauVM::settop);
    ClassDB::bind_method(D_METHOD("status"), &LuauVM::status);
    ClassDB::bind_method(D_METHOD("toboolean", "index"), &LuauVM::toboolean);
    ClassDB::bind_method(D_METHOD("tointeger", "index"), &LuauVM::tointeger);
    ClassDB::bind_method(D_METHOD("tonumber", "index"), &LuauVM::tonumber);
    ClassDB::bind_method(D_METHOD("tostring", "index"), &LuauVM::tostring);
    ClassDB::bind_method(D_METHOD("type", "index"), &LuauVM::type);
    ClassDB::bind_method(D_METHOD("luatypename", "type"), &LuauVM::luatypename);
    ClassDB::bind_method(D_METHOD("getupvalue", "index", "n"), &LuauVM::getupvalue);
    ClassDB::bind_method(D_METHOD("setupvalue", "index", "n"), &LuauVM::setupvalue);
}


void LuauVM::luacall(int nargs, int nresults) {
    lua_call(L, nargs, nresults);
}

void LuauVM::concat(int n) {
    lua_concat(L, n);
}

void LuauVM::createtable(int narr, int nrec) {
    lua_createtable(L, narr, nrec);
}

void LuauVM::equal(int index1, int index2) {
    lua_equal(L, index1, index2);
}

void LuauVM::error() {
    lua_error(L);
}

void LuauVM::getfenv(int index) {
    lua_getfenv(L, index);
}

void LuauVM::getfield(int index, const String &key) {
    lua_getfield(L, index, key.ascii().get_data()); // Returns type..?
}

void LuauVM::getglobal(const String &key) {
    lua_getfield(L, LUA_GLOBALSINDEX, key.ascii().get_data());
}

bool LuauVM::getmetatable(int index) {
    return lua_getmetatable(L, index);
}

void LuauVM::gettable(int index) {
    lua_gettable(L, index); // Returns type..?
}

int LuauVM::gettop() {
    return lua_gettop(L);
}

void LuauVM::insert(int index) {
    lua_insert(L, index);
}

bool LuauVM::isboolean(int index) {
    return lua_isboolean(L, index);
}

bool LuauVM::isfunction(int index) {
	return lua_isfunction(L, index);
}

bool LuauVM::isnil(int index) {
	return lua_isnil(L, index);
}

bool LuauVM::isnone(int index) {
	return lua_isnone(L, index);
}

bool LuauVM::isnoneornil(int index) {
	return lua_isnoneornil(L, index);
}

bool LuauVM::isnumber(int index) {
	return lua_isnumber(L, index);
}

bool LuauVM::isstring(int index) {
	return lua_isstring(L, index);
}

bool LuauVM::istable(int index) {
	return lua_istable(L, index);
}

bool LuauVM::isthread(int index) {
	return lua_isthread(L, index);
}

bool LuauVM::isuserdata(int index) {
	return lua_isuserdata(L, index);
}

bool LuauVM::lessthan(int index1, int index2) {
    return lua_lessthan(L, index1, index2);
}

void LuauVM::newtable() {
    lua_newtable(L);
}

/*
void LuauVM::newuserdata(int size) {
    lua_newuserdata(L, size);
}
*/

bool LuauVM::next(int index) {
    return lua_next(L, index);
}

int LuauVM::objlen(int index) {
    return lua_objlen(L, index);
}

void LuauVM::pcall(int nargs, int nresults, int errfunc) {
    lua_pcall(L, nargs, nresults, errfunc);
}

void LuauVM::pop(int n) {
    lua_pop(L, n);
}

void LuauVM::pushnil() {
    lua_pushnil(L);
}

void LuauVM::pushboolean(bool b) {
    lua_pushboolean(L, b);
}

void LuauVM::pushinteger(int x) {
    lua_pushinteger(L, x);
}

void LuauVM::pushnumber(float x) {
    lua_pushnumber(L, x);
}

void LuauVM::pushstring(const String &s) {
    lua_pushstring(L, s.ascii().get_data());
}

void LuauVM::pushvalue(int index) {
    lua_pushvalue(L, index);
}

bool LuauVM::rawequal(int index1, int index2) {
    return lua_rawequal(L, index1, index2) != 0;
}

void LuauVM::rawget(int index) {
    lua_rawget(L, index);
}

void LuauVM::rawgeti(int index, int n) {
    lua_rawgeti(L, index, n);
}

void LuauVM::rawset(int index) {
    lua_rawset(L, index);
}

void LuauVM::rawseti(int index, int n) {
    lua_rawseti(L, index, n);
}

void LuauVM::remove(int index) {
    lua_remove(L, index);
}

void LuauVM::replace(int index) {
    lua_replace(L, index);
}

bool LuauVM::setfenv(int index) {
    return lua_setfenv(L, index);
}

void LuauVM::setfield(int index, const String &key) {
    lua_setfield(L, index, key.ascii().get_data());
}

void LuauVM::setglobal(const String &key) {
    lua_setglobal(L, key.ascii().get_data());
}

bool LuauVM::setmetatable(int index) {
    return lua_setmetatable(L, index);
}

void LuauVM::settable(int index) {
    lua_settable(L, index);
}

void LuauVM::settop(int index) {
    lua_settop(L, index);
}

// TODO: Add constants
int LuauVM::status() {
    return lua_status(L);
}

bool LuauVM::toboolean(int index) {
	return lua_toboolean(L, index);
}

int LuauVM::tointeger(int index) {
	return lua_tointeger(L, index);
}

double LuauVM::tonumber(int index) {
	return lua_tonumber(L, index);
}

String LuauVM::tostring(int index) {
	return String(lua_tostring(L, index));
}

// TODO: Add constants
int LuauVM::type(int index) {
    return lua_type(L, index);
}

String LuauVM::luatypename(int type) {
    return String(lua_typename(L, type));
}

String LuauVM::getupvalue(int index, int n) {
    return String(lua_getupvalue(L, index, n));
}

String LuauVM::setupvalue(int index, int n) {
    return String(lua_setupvalue(L, index, n));
}


/*
TODO:
// lua_gc
// lua_resume
// lua_touserdata
// lua_gethook
// lua_gethookcount
// lua_gethookmask
// lua_getinfo
// lua_getlocal
// lua_getstack
// lua_sethook
// lua_setlocal

luaL_callmeta
// luaL_checkany
// luaL_checkint
// luaL_checkinteger
// luaL_checklong
// luaL_checklstring
// luaL_checknumber
// luaL_checkoption
// luaL_checkstack
// luaL_checkstring
// luaL_checktype
// luaL_checkudata
luaL_dofile
luaL_dostring
// luaL_error
luaL_getmetafield
luaL_getmetatable
luaL_newmetatable
// luaL_optint
// luaL_optinteger
// luaL_optlong
// luaL_optlstring
// luaL_optnumber
// luaL_optstring
luaL_ref
luaL_register
luaL_typename
luaL_typerror
luaL_unref
luaL_where
*/