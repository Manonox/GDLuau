#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <lua.h>
#include <lualib.h>
#include <luacode.h>

#include <classes/luau_function.h>
#include <utils.h>
#include <vector_lib.h>


#define GDLUAU_REGISTRY_NODE_KEY "gdluau_node"


namespace godot {

class LuauVM : public Node {
    GDCLASS(LuauVM, Node)

private:
    lua_State* L;
    void create_metatables();

protected:
    static void _bind_methods();
    static void _bind_passthrough_methods();

public:
    LuauVM();
    ~LuauVM();

    void register_print();

    int load_string(const String &code, const String &chunkname);
    int do_string(const String &code, const String &chunkname);

    void open_libraries(const Array &libraries);
    void open_all_libraries();

    int64_t get_memory_usage_bytes();


    // Bindings

    #pragma region GDLuau

    void lua_pushvariant(const Variant &var);
    void lua_pusharray(const Array &array);
    void lua_pushdictionary(const Dictionary &dictionary);

    Variant lua_tovariant(int index);
    Array lua_toarray(int index);
    Dictionary lua_todictionary(int index);
    
    Ref<LuauFunction> lua_tofunction(int index);
    Error lua_pushcallable(const Callable &callable);

    void lua_pushobject(Object *node);
    Object *lua_toobject(int idx);
    bool lua_isobject(int idx);

    #pragma endregion

    #pragma region Default
    void lua_setreadonly(int index, bool enabled);

    void lua_call(int nargs, int nresults);
    void lua_concat(int n);
    void lua_createtable(int narr, int nrec);
    void lua_equal(int index1, int index2);
    void lua_error();
    void lua_getfenv(int index);
    int lua_getfield(int index, const String &key);
    void (lua_getglobal)(const String &key);
    bool lua_getmetatable(int index);
    int lua_gettable(int index);
    int lua_gettop();
    void lua_insert(int index);
    bool (lua_isboolean)(int index);
    bool (lua_isfunction)(int index);
    bool (lua_isnil)(int index);
    bool (lua_isnone)(int index);
    bool (lua_isnoneornil)(int index);
    bool lua_isnumber(int index);
    bool lua_isinteger(int index);
    bool(lua_isnumberx)(int index);
    bool lua_isstring(int index);
    bool (lua_istable)(int index);
    bool (lua_isthread)(int index);
    bool lua_isuserdata(int index);
    bool lua_lessthan(int index1, int index2);
    void (lua_newtable)();
    void (lua_newuserdata)(int size);
    bool lua_next(int index);
    int64_t lua_objlen(int index);
    int lua_pcall(int nargs, int nresults, int errfunc);
    void (lua_pop)(int n);
    void lua_pushnil();
    void lua_pushboolean(bool b);
    void lua_pushinteger(int x);
    void lua_pushnumber(float x);
    void lua_pushstring(const String &s);
    #if LUA_VECTOR_SIZE == 4
        void lua_pushvector(Vector4 v);
    #else
        void lua_pushvector(Vector3 v);
    #endif
    void lua_pushvalue(int index);
    bool lua_rawequal(int index1, int index2);
    void lua_rawget(int index);
    void lua_rawgeti(int index, int n);
    void lua_rawgetfield(int index, const String &key);
    void lua_rawset(int index);
    void lua_rawseti(int index, int n);
    void lua_rawsetfield(int index, const String &key);
    void lua_remove(int index);
    void lua_replace(int index);
    bool lua_setfenv(int index);
    void lua_setfield(int index, const String &key);
    void (lua_setglobal)(const String &key);
    bool lua_setmetatable(int index);
    void lua_settable(int index);
    void lua_settop(int index);

    int lua_status();
    bool lua_toboolean(int index);
    int64_t (lua_tointeger)(int index);
    double (lua_tonumber)(int index);
    String (lua_tostring)(int index);
    Vector3 lua_tovector(int index);
    int lua_type(int index);
    String lua_typename(int type);
    String lua_getupvalue(int index, int n);
    String lua_setupvalue(int index, int n);
    int lua_gc(int what, int data);

    int lua_ref(int t);
    void lua_unref(int ref);
    int (lua_getref)(int ref);

    #pragma endregion

    #pragma region Auxiliary

    bool luaL_hasmetatable(int index, const String &tname);

    void(luaL_error)(const String &err);
    bool luaL_callmeta(int obj, const String &field);
    bool luaL_getmetafield(int obj, const String &field);
    bool luaL_newmetatable(const String &tname);
    bool (luaL_getmetatable)(const String &tname);
    void luaL_where(int lvl);
    void luaL_typerror(int nargs, const String &tname);
    void (luaL_argcheck)(bool cond, int narg, const String &msg);

    void luaL_checkany(int narg);
    int luaL_checkint(int narg);
    double luaL_checknumber(int narg);
    String(luaL_checkstring)(int narg);
    Vector3 luaL_checkvector(int narg);
    Object *luaL_checkobject(int narg, bool valid);
    void luaL_checktype(int narg, int type);
    void luaL_checkstack(int sz, const String &messsage);
    int luaL_checkoption(int narg, const Array &array, const String &def);

    #pragma endregion
};
}

void lua_setnode(lua_State* L, godot::LuauVM* node);
godot::LuauVM* lua_getnode(lua_State* L);

VARIANT_ENUM_CAST(lua_Status);
VARIANT_ENUM_CAST(lua_Type);
VARIANT_ENUM_CAST(lua_GCOp);

int luaopen_vector(lua_State *L);

