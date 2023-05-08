#ifndef LUAU_VM_H
#define LUAU_VM_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <lua.h>
#include <lualib.h>
#include <luacode.h>
#include <classes/luau_function.h>


#define LUA_VECLIBNAME "vector"
#define LUAUGD_REGISTRY_NODE_KEY "starscade_node"


namespace godot {

class LuauVM : public Node {
    GDCLASS(LuauVM, Node)

private:
    lua_State* L;
    const char *last_lua_error = nullptr;

protected:
    static void _bind_methods();
    static void _bind_passthrough_methods();

public:
    LuauVM();
    ~LuauVM();
    
    void register_lua_print();

    bool do_string(const String &code, const String &chunkname);
    String get_lua_error();

    void open_libraries(const Array &libraries);
    void open_all_libraries();

    void pushvariant(const Variant &var);
    void pusharray(const Array &array);
    void pushdictionary(const Dictionary &dictionary);

    Variant tovariant(int index);
    Array toarray(int index);
    Dictionary todictionary(int index);


    void luacall(int nargs, int nresults);
    void concat(int n);
    void createtable(int narr, int nrec);
    void equal(int index1, int index2);
    void error();
    void getfenv(int index);
    void getfield(int index, const String &key);
    void getglobal(const String &key);
    bool getmetatable(int index);
    void gettable(int index);
    int gettop();
    void insert(int index);
    bool isboolean(int index);
    bool isfunction(int index);
    bool isnil(int index);
    bool isnone(int index);
    bool isnoneornil(int index);
    bool isnumber(int index);
    bool isstring(int index);
    bool istable(int index);
    bool isthread(int index);
    bool isuserdata(int index);
    bool lessthan(int index1, int index2);
    void newtable();
    void newuserdata(int size);
    bool next(int index);
    int objlen(int index);
    void pcall(int nargs, int nresults, int errfunc);
    void pop(int n);
    void pushnil();
    void pushboolean(bool b);
    void pushinteger(int x);
    void pushnumber(float x);
    void pushstring(const String &s);
    void pushvalue(int index);
    bool rawequal(int index1, int index2);
    void rawget(int index);
    void rawgeti(int index, int n);
    void rawset(int index);
    void rawseti(int index, int n);
    void remove(int index);
    void replace(int index);
    bool setfenv(int index);
    void setfield(int index, const String &key);
    void setglobal(const String &key);
    bool setmetatable(int index);
    void settable(int index);
    void settop(int index);
    int status();
    bool toboolean(int index);
    int tointeger(int index);
    double tonumber(int index);
    String tostring(int index);
    Ref<LuauFunction> tofunction(int index);
    
    int type(int index);
    String luatypename(int type);

    String getupvalue(int index, int n);
    String setupvalue(int index, int n);
};

}

int luaopen_vector(lua_State *L);

#endif
