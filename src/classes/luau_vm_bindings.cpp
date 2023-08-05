#include <classes/luau_vm.h>
#include <utils.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>
#include "luau_vm.h"


using namespace godot;


void LuauVM::_bind_passthrough_methods() {
    
    // Constants
    {
        // Status
        BIND_ENUM_CONSTANT(LUA_OK);
        BIND_ENUM_CONSTANT(LUA_YIELD);
        BIND_ENUM_CONSTANT(LUA_BREAK);
        BIND_ENUM_CONSTANT(LUA_ERRERR);
        BIND_ENUM_CONSTANT(LUA_ERRMEM);
        BIND_ENUM_CONSTANT(LUA_ERRRUN);
        BIND_ENUM_CONSTANT(LUA_ERRSYNTAX);

        // Types
        BIND_ENUM_CONSTANT(LUA_TNIL);
        BIND_ENUM_CONSTANT(LUA_TBOOLEAN);
        BIND_ENUM_CONSTANT(LUA_TLIGHTUSERDATA);
        BIND_ENUM_CONSTANT(LUA_TNUMBER);
        BIND_ENUM_CONSTANT(LUA_TVECTOR);
        BIND_ENUM_CONSTANT(LUA_TSTRING);
        BIND_ENUM_CONSTANT(LUA_TTABLE);
        BIND_ENUM_CONSTANT(LUA_TFUNCTION);
        BIND_ENUM_CONSTANT(LUA_TUSERDATA);
        BIND_ENUM_CONSTANT(LUA_TTHREAD);
        // BIND_ENUM_CONSTANT(LUA_TPROTO);
        // BIND_ENUM_CONSTANT(LUA_TUPVAL);
        // BIND_ENUM_CONSTANT(LUA_TDEADKEY);
        // BIND_ENUM_CONSTANT(LUA_T_COUNT);

        // GC Operations
        BIND_ENUM_CONSTANT(LUA_GCSTOP);
        BIND_ENUM_CONSTANT(LUA_GCRESTART);
        BIND_ENUM_CONSTANT(LUA_GCCOLLECT);
        BIND_ENUM_CONSTANT(LUA_GCCOUNT);
        BIND_ENUM_CONSTANT(LUA_GCCOUNTB);
        BIND_ENUM_CONSTANT(LUA_GCISRUNNING);
        BIND_ENUM_CONSTANT(LUA_GCSTEP);
        BIND_ENUM_CONSTANT(LUA_GCSETGOAL);
        BIND_ENUM_CONSTANT(LUA_GCSETSTEPMUL);
        BIND_ENUM_CONSTANT(LUA_GCSETSTEPSIZE);

        // Other
        BIND_ENUM_CONSTANT(LUA_MULTRET);
    }


    // Unique to GDLuau
    {
        ClassDB::bind_method(D_METHOD("lua_pushvariant", "var"), &LuauVM::lua_pushvariant);
        ClassDB::bind_method(D_METHOD("lua_pusharray", "arr"), &LuauVM::lua_pusharray);
        ClassDB::bind_method(D_METHOD("lua_pushdictionary", "dict"), &LuauVM::lua_pushdictionary);

        ClassDB::bind_method(D_METHOD("lua_tovariant", "index"), &LuauVM::lua_tovariant);
        ClassDB::bind_method(D_METHOD("lua_toarray", "index"), &LuauVM::lua_toarray);
        ClassDB::bind_method(D_METHOD("lua_todictionary", "index"), &LuauVM::lua_todictionary);

        ClassDB::bind_method(D_METHOD("lua_tofunction", "index"), &LuauVM::lua_tofunction);
        ClassDB::bind_method(D_METHOD("lua_pushcallable", "func"), &LuauVM::lua_pushcallable);
        ClassDB::bind_method(D_METHOD("lua_pushfunction", "func"), &LuauVM::lua_pushcallable);
        ClassDB::bind_method(D_METHOD("lua_pushfunc", "func"), &LuauVM::lua_pushcallable);
    }

    // Default library
    {
        ClassDB::bind_method(D_METHOD("lua_loadstring", "code", "chunkname"), &LuauVM::load_string, DEFVAL("loadstring"));
        ClassDB::bind_method(D_METHOD("lua_dostring", "code", "chunkname"), &LuauVM::do_string, DEFVAL("dostring"));

        ClassDB::bind_method(D_METHOD("lua_concat", "n"), &LuauVM::lua_concat);
        ClassDB::bind_method(D_METHOD("lua_call", "nargs", "nresults"), &LuauVM::lua_call);
        ClassDB::bind_method(D_METHOD("lua_concat", "n"), &LuauVM::lua_concat);
        ClassDB::bind_method(D_METHOD("lua_createtable", "narr", "nrec"), &LuauVM::lua_createtable);
        ClassDB::bind_method(D_METHOD("lua_equal", "index1", "index2"), &LuauVM::lua_equal);
        ClassDB::bind_method(D_METHOD("lua_error"), &LuauVM::lua_error);
        ClassDB::bind_method(D_METHOD("lua_getfenv", "index"), &LuauVM::lua_getfenv);
        ClassDB::bind_method(D_METHOD("lua_getfield", "index", "key"), &LuauVM::lua_getfield);
        ClassDB::bind_method(D_METHOD("lua_getglobal", "key"), &LuauVM::lua_getglobal);
        ClassDB::bind_method(D_METHOD("lua_getmetatable", "index"), &LuauVM::lua_getmetatable);
        ClassDB::bind_method(D_METHOD("lua_gettable", "index"), &LuauVM::lua_gettable);
        ClassDB::bind_method(D_METHOD("lua_gettop"), &LuauVM::lua_gettop);
        ClassDB::bind_method(D_METHOD("lua_insert", "index"), &LuauVM::lua_insert);
        ClassDB::bind_method(D_METHOD("lua_isboolean", "index"), &LuauVM::lua_isboolean);
        ClassDB::bind_method(D_METHOD("lua_isfunction", "index"), &LuauVM::lua_isfunction);
        ClassDB::bind_method(D_METHOD("lua_isnil", "index"), &LuauVM::lua_isnil);
        ClassDB::bind_method(D_METHOD("lua_isnone", "index"), &LuauVM::lua_isnone);
        ClassDB::bind_method(D_METHOD("lua_isnoneornil", "index"), &LuauVM::lua_isnoneornil);
        ClassDB::bind_method(D_METHOD("lua_isnumber", "index"), &LuauVM::lua_isnumber);
        ClassDB::bind_method(D_METHOD("lua_isstring", "index"), &LuauVM::lua_isstring);
        ClassDB::bind_method(D_METHOD("lua_istable", "index"), &LuauVM::lua_istable);
        ClassDB::bind_method(D_METHOD("lua_isthread", "index"), &LuauVM::lua_isthread);
        ClassDB::bind_method(D_METHOD("lua_isuserdata", "index"), &LuauVM::lua_isuserdata);
        ClassDB::bind_method(D_METHOD("lua_lessthan", "index1", "index2"), &LuauVM::lua_lessthan);
        ClassDB::bind_method(D_METHOD("lua_newtable"), &LuauVM::lua_newtable);
        ClassDB::bind_method(D_METHOD("lua_next", "index"), &LuauVM::lua_next);
        ClassDB::bind_method(D_METHOD("lua_objlen", "index"), &LuauVM::lua_objlen);
        ClassDB::bind_method(D_METHOD("lua_pcall", "nargs", "nresults", "errfunc"), &LuauVM::lua_pcall, DEFVAL(0));
        ClassDB::bind_method(D_METHOD("lua_pop", "n"), &LuauVM::lua_pop, DEFVAL(1));
        ClassDB::bind_method(D_METHOD("lua_pushnil"), &LuauVM::lua_pushnil);
        ClassDB::bind_method(D_METHOD("lua_pushboolean", "b"), &LuauVM::lua_pushboolean);
        ClassDB::bind_method(D_METHOD("lua_pushinteger", "x"), &LuauVM::lua_pushinteger);
        ClassDB::bind_method(D_METHOD("lua_pushnumber", "x"), &LuauVM::lua_pushnumber);
        ClassDB::bind_method(D_METHOD("lua_pushstring", "s"), &LuauVM::lua_pushstring);
        ClassDB::bind_method(D_METHOD("lua_pushvector", "v"), &LuauVM::lua_pushvector);    
        ClassDB::bind_method(D_METHOD("lua_pushvalue", "index"), &LuauVM::lua_pushvalue);
        ClassDB::bind_method(D_METHOD("lua_rawequal", "index1", "index2"), &LuauVM::lua_rawequal);
        ClassDB::bind_method(D_METHOD("lua_rawget", "index"), &LuauVM::lua_rawget);
        ClassDB::bind_method(D_METHOD("lua_rawgeti", "index", "n"), &LuauVM::lua_rawgeti);
        ClassDB::bind_method(D_METHOD("lua_rawset", "index"), &LuauVM::lua_rawset);
        ClassDB::bind_method(D_METHOD("lua_rawseti", "index", "n"), &LuauVM::lua_rawseti);
        ClassDB::bind_method(D_METHOD("lua_remove", "index"), &LuauVM::lua_remove);
        ClassDB::bind_method(D_METHOD("lua_replace", "index"), &LuauVM::lua_replace);
        ClassDB::bind_method(D_METHOD("lua_setfenv", "index"), &LuauVM::lua_setfenv);
        ClassDB::bind_method(D_METHOD("lua_setfield", "index", "key"), &LuauVM::lua_setfield);
        ClassDB::bind_method(D_METHOD("lua_setglobal", "key"), &LuauVM::lua_setglobal);
        ClassDB::bind_method(D_METHOD("lua_setmetatable", "index"), &LuauVM::lua_setmetatable);
        ClassDB::bind_method(D_METHOD("lua_settable", "index"), &LuauVM::lua_settable);
        ClassDB::bind_method(D_METHOD("lua_settop", "index"), &LuauVM::lua_settop);
        ClassDB::bind_method(D_METHOD("lua_status"), &LuauVM::lua_status);
        ClassDB::bind_method(D_METHOD("lua_toboolean", "index"), &LuauVM::lua_toboolean);
        ClassDB::bind_method(D_METHOD("lua_tointeger", "index"), &LuauVM::lua_tointeger);
        ClassDB::bind_method(D_METHOD("lua_tonumber", "index"), &LuauVM::lua_tonumber);
        ClassDB::bind_method(D_METHOD("lua_tostring", "index"), &LuauVM::lua_tostring);
        ClassDB::bind_method(D_METHOD("lua_type", "index"), &LuauVM::lua_type);
        ClassDB::bind_method(D_METHOD("lua_typename", "type"), &LuauVM::lua_typename);
        ClassDB::bind_method(D_METHOD("lua_getupvalue", "index", "n"), &LuauVM::lua_getupvalue);
        ClassDB::bind_method(D_METHOD("lua_setupvalue", "index", "n"), &LuauVM::lua_setupvalue);
        ClassDB::bind_method(D_METHOD("lua_gc", "what", "data"), &LuauVM::lua_gc, DEFVAL(0));
        
        ClassDB::bind_method(D_METHOD("lua_ref", "t"), &LuauVM::lua_ref);
        ClassDB::bind_method(D_METHOD("lua_unref", "ref"), &LuauVM::lua_unref);

        ClassDB::bind_method(D_METHOD("luaL_ref", "t"), &LuauVM::lua_ref);
        ClassDB::bind_method(D_METHOD("luaL_unref", "ref"), &LuauVM::lua_unref);
    }

    // Auxiliary library
    {
        ClassDB::bind_method(D_METHOD("luaL_error", "s"), &LuauVM::luaL_error);
        ClassDB::bind_method(D_METHOD("luaL_callmeta", "obj", "field"), &LuauVM::luaL_callmeta);
        ClassDB::bind_method(D_METHOD("luaL_getmetafield", "obj", "field"), &LuauVM::luaL_getmetafield);
        ClassDB::bind_method(D_METHOD("luaL_getmetatable", "tname"), &LuauVM::luaL_getmetatable);
        ClassDB::bind_method(D_METHOD("luaL_newmetatable", "tname"), &LuauVM::luaL_newmetatable);
        ClassDB::bind_method(D_METHOD("luaL_where", "lvl"), &LuauVM::luaL_where);
        ClassDB::bind_method(D_METHOD("luaL_typerror", "narg", "tname"), &LuauVM::luaL_typerror);

        ClassDB::bind_method(D_METHOD("luaL_checkany", "narg"), &LuauVM::luaL_checkany);
        ClassDB::bind_method(D_METHOD("luaL_checkint", "narg"), &LuauVM::luaL_checkint);
        ClassDB::bind_method(D_METHOD("luaL_checkstring", "narg"), &LuauVM::luaL_checkstring);
        ClassDB::bind_method(D_METHOD("luaL_checknumber", "narg"), &LuauVM::luaL_checknumber);
        ClassDB::bind_method(D_METHOD("luaL_checktype", "narg", "type"), &LuauVM::luaL_checktype);
        ClassDB::bind_method(D_METHOD("luaL_checkstack", "size", "message"), &LuauVM::luaL_checkstack);
        ClassDB::bind_method(D_METHOD("luaL_checkoption", "narg", "type"), &LuauVM::luaL_checkoption);
    }
}


#pragma region GDLuau

void LuauVM::lua_pushvariant(const Variant &var) {
    ::lua_pushvariant(L, var);
}

void LuauVM::lua_pusharray(const Array &array) {
    ::lua_pusharray(L, array);
}

void LuauVM::lua_pushdictionary(const Dictionary &dictionary) {
    ::lua_pushdictionary(L, dictionary);
}

Variant LuauVM::lua_tovariant(int index) {
    return ::lua_tovariant(L, index);
}

Array LuauVM::lua_toarray(int index) {
    return ::lua_toarray(L, index);
}

Dictionary LuauVM::lua_todictionary(int index) {
    return ::lua_todictionary(L, index);
}

Ref<LuauFunction> LuauVM::lua_tofunction(int index) {
    return ::lua_tofunction(L, index);
}

Error LuauVM::lua_pushcallable(const Callable &callable) {
    ERR_FAIL_COND_V_MSG(callable.is_custom(), ERR_INVALID_PARAMETER, "Cannot push callable, lambda callables aren't supported.");
    ERR_FAIL_COND_V_MSG(callable.is_null(), ERR_INVALID_PARAMETER, "Cannot push callable, the provided callable is invalid.");
    ::lua_pushcallable(L, callable);
    return OK;
}

#pragma endregion


#pragma region Default

void LuauVM::lua_call(int nargs, int nresults) {
    ::lua_call(L, nargs, nresults);
}

void LuauVM::lua_concat(int n) {
    ::lua_concat(L, n);
}

void LuauVM::lua_createtable(int narr, int nrec) {
    ::lua_createtable(L, narr, nrec);
}

void LuauVM::lua_equal(int index1, int index2) {
    ::lua_equal(L, index1, index2);
}

void LuauVM::lua_error() {
    ::lua_error(L);
}

void LuauVM::lua_getfenv(int index) {
    ::lua_getfenv(L, index);
}

int LuauVM::lua_getfield(int index, const String &key) {
    return ::lua_getfield(L, index, key.ascii().get_data());
}

void (LuauVM::lua_getglobal)(const String &key) {
    ::lua_getfield(L, LUA_GLOBALSINDEX, key.ascii().get_data());
}

bool LuauVM::lua_getmetatable(int index) {
    return ::lua_getmetatable(L, index) != 0;
}

int LuauVM::lua_gettable(int index) {
    return ::lua_gettable(L, index);
}

int LuauVM::lua_gettop() {
    return ::lua_gettop(L);
}

void LuauVM::lua_insert(int index) {
    ::lua_insert(L, index);
}

bool (LuauVM::lua_isboolean)(int index) {
    return lua_type(index) == LUA_TBOOLEAN;
}

bool (LuauVM::lua_isfunction)(int index) {
	return lua_type(index) == LUA_TFUNCTION;
}

bool (LuauVM::lua_isnil)(int index) {
	return lua_type(index) == LUA_TNIL;
}

bool (LuauVM::lua_isnone)(int index) {
	return lua_type(index) == LUA_TNONE;
}

bool (LuauVM::lua_isnoneornil)(int index) {
	return lua_type(index) <= LUA_TNIL;
}

bool LuauVM::lua_isnumber(int index) {
	return ::lua_isnumber(L, index);
}

bool LuauVM::lua_isstring(int index) {
	return ::lua_isstring(L, index);
}

bool (LuauVM::lua_istable)(int index) {
	return lua_type(index) == LUA_TTABLE;
}

bool (LuauVM::lua_isthread)(int index) {
	return lua_type(index) == LUA_TTHREAD;
}

bool LuauVM::lua_isuserdata(int index) {
	return ::lua_isuserdata(L, index);
}

bool LuauVM::lua_lessthan(int index1, int index2) {
    return ::lua_lessthan(L, index1, index2);
}

void (LuauVM::lua_newtable)() {
    ::lua_newtable(L);
}

bool LuauVM::lua_next(int index) {
    return ::lua_next(L, index);
}

int64_t LuauVM::lua_objlen(int index) {
    return ::lua_objlen(L, index);
}

int LuauVM::lua_pcall(int nargs, int nresults, int errfunc) {
    return ::lua_pcall(L, nargs, nresults, errfunc);
}

void (LuauVM::lua_pop)(int n) {
    ::lua_pop(L, n);
}

void LuauVM::lua_pushnil() {
    ::lua_pushnil(L);
}

void LuauVM::lua_pushboolean(bool b) {
    ::lua_pushboolean(L, b);
}

void LuauVM::lua_pushinteger(int x) {
    ::lua_pushinteger(L, x);
}

void LuauVM::lua_pushnumber(float x) {
    ::lua_pushnumber(L, x);
}

void LuauVM::lua_pushstring(const String &s) {
    ::lua_pushstring(L, s.ascii().get_data());
}

#if LUA_VECTOR_SIZE == 4
void LuauVM::lua_pushvector(Vector4 v) {
    ::lua_pushvector(L, v.x, v.y, v.z, v.w);
}
#else
void LuauVM::lua_pushvector(Vector3 v) {
    ::lua_pushvector(L, v.x, v.y, v.z);
}
#endif

void LuauVM::lua_pushvalue(int index) {
    ::lua_pushvalue(L, index);
}

bool LuauVM::lua_rawequal(int index1, int index2) {
    return ::lua_rawequal(L, index1, index2) != 0;
}

void LuauVM::lua_rawget(int index) {
    ::lua_rawget(L, index);
}

void LuauVM::lua_rawgeti(int index, int n) {
    ::lua_rawgeti(L, index, n);
}

void LuauVM::lua_rawset(int index) {
    ::lua_rawset(L, index);
}

void LuauVM::lua_rawseti(int index, int n) {
    ::lua_rawseti(L, index, n);
}

void LuauVM::lua_remove(int index) {
    ::lua_remove(L, index);
}

void LuauVM::lua_replace(int index) {
    ::lua_replace(L, index);
}

bool LuauVM::lua_setfenv(int index) {
    return ::lua_setfenv(L, index);
}

void LuauVM::lua_setfield(int index, const String &key) {
    ::lua_setfield(L, index, key.ascii().get_data());
}

void (LuauVM::lua_setglobal)(const String &key) {
    ::lua_setglobal(L, key.ascii().get_data());
}

bool LuauVM::lua_setmetatable(int index) {
    return ::lua_setmetatable(L, index);
}

void LuauVM::lua_settable(int index) {
    ::lua_settable(L, index);
}

void LuauVM::lua_settop(int index) {
    ::lua_settop(L, index);
}

int LuauVM::lua_status() {
    return ::lua_status(L);
}

bool LuauVM::lua_toboolean(int index) {
	return ::lua_toboolean(L, index);
}

int64_t (LuauVM::lua_tointeger)(int index) {
	return ::lua_tointeger(L, index);
}

double (LuauVM::lua_tonumber)(int index) {
	return ::lua_tonumber(L, index);
}

String (LuauVM::lua_tostring)(int index) {
	return String(::lua_tostring(L, index));
}

int LuauVM::lua_type(int index) {
    return ::lua_type(L, index);
}

String LuauVM::lua_typename(int type) {
    return String(::lua_typename(L, type));
}

String LuauVM::lua_getupvalue(int index, int n) {
    return String(::lua_getupvalue(L, index, n));
}

String LuauVM::lua_setupvalue(int index, int n) {
    return String(::lua_setupvalue(L, index, n));
}

int LuauVM::lua_gc(int what, int data) {
    return ::lua_gc(L, what, data);
}

int LuauVM::lua_ref(int t) {
    return ::lua_ref(L, t);
}

void LuauVM::lua_unref(int ref) {
    ::lua_unref(L, ref);
}

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

#pragma endregion


#pragma region Auxiliary

void (LuauVM::luaL_error)(const String &err) {
    ::luaL_error(L, err.ascii().get_data());
}

bool LuauVM::luaL_callmeta(int obj, const String &field) {
    return ::luaL_callmeta(L, obj, field.ascii().get_data());
}

bool LuauVM::luaL_getmetafield(int obj, const String &field) {
    return ::luaL_getmetafield(L, obj, field.ascii().get_data());
}

bool (LuauVM::luaL_getmetatable)(const String &tname) {
    return ::lua_getfield(L, LUA_REGISTRYINDEX, tname.ascii().get_data());
}

bool LuauVM::luaL_newmetatable(const String &tname) {
    return ::luaL_newmetatable(L, tname.ascii().get_data());
}

void LuauVM::luaL_where(int lvl) {
    ::luaL_where(L, lvl);
}

void LuauVM::luaL_typerror(int nargs, const String &tname) {
    ::luaL_typeerror(L, nargs, tname.ascii().get_data());
}

void LuauVM::luaL_checkany(int narg) {
    ::luaL_checkany(L, narg);
}

int LuauVM::luaL_checkint(int narg) {
    return ::luaL_checkinteger(L, narg);
}

double LuauVM::luaL_checknumber(int narg) {
    return ::luaL_checknumber(L, narg);
}

String (LuauVM::luaL_checkstring)(int narg) {
    return ::luaL_checklstring(L, narg, NULL);
}


#if LUA_VECTOR_SIZE == 4
Vector4 LuauVM::luaL_checkvector(int narg) {
    const float *v = ::luaL_checkvector(L, narg);
    return Vector4(v[0], v[1], v[2], v[3]);
}
#else
Vector3 LuauVM::luaL_checkvector(int narg) {
    const float *v = ::luaL_checkvector(L, narg);
    return Vector3(v[0], v[1], v[2]);
}
#endif

void LuauVM::luaL_checktype(int narg, int type) {
    ::luaL_checktype(L, narg, type);
}

void LuauVM::luaL_checkstack(int sz, const String &messsage) {
    ::luaL_checkstack(L, sz, messsage.ascii().get_data());
}

// luaL_checkudata

int LuauVM::luaL_checkoption(int narg, const String &def, const Array &array) {
    auto size = array.size();
    const char * * lst = (const char * *)memalloc(sizeof(const char *) * size);
    for (int64_t i = 0; i < size; i++) {
        Variant v = array[i];
        switch (v.get_type()) {
            case Variant::Type::STRING:
            case Variant::Type::STRING_NAME:
                lst[i] = (v.operator godot::String()).ascii().get_data();
            default:
                lst[i] = "";
        }
    }
    
    int result = ::luaL_checkoption(L, narg, def.ascii().get_data(), lst);
    memfree(lst);

    return result;
}


// luaL_optint
// luaL_optinteger
// luaL_optlong
// luaL_optlstring
// luaL_optnumber
// luaL_optstring


#pragma endregion
