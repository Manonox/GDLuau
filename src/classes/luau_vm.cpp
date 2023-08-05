#include <classes/luau_vm.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <utils.h>



void lua_setnode(lua_State* L, godot::LuauVM* node) {
    lua_pushstring(L, GDLUAU_REGISTRY_NODE_KEY);
    lua_pushlightuserdata(L, node);
    lua_settable(L, LUA_REGISTRYINDEX);
}

godot::LuauVM* lua_getnode(lua_State* L) {
    lua_pushstring(L, GDLUAU_REGISTRY_NODE_KEY);
    lua_gettable(L, LUA_REGISTRYINDEX);
    if (!lua_islightuserdata(L, -1)) {
        lua_pop(L, 1);
        return nullptr;
    }
    void *userdata = lua_tolightuserdata(L, -1);
    lua_pop(L, 1);
    return reinterpret_cast<godot::LuauVM*>(userdata);
}

using namespace godot;


// Source: https://github.com/Fumohouse/godot-luau-script
// Based on the default implementation seen in the Lua 5.1 reference
static void *lua_alloc(void *, void *ptr, size_t, size_t nsize) {
    if (nsize == 0) {
        // Lua assumes free(NULL) is ok. For Godot it is not.
        if (ptr)
            memfree(ptr);

        return nullptr;
    }

    return memrealloc(ptr, nsize);
}

LuauVM::LuauVM() {
    L = lua_newstate(lua_alloc, nullptr);
    lua_setnode(L, this);
}

LuauVM::~LuauVM() {
    lua_close(L);
}

void LuauVM::_bind_methods() {
    BIND_VIRTUAL_METHOD(LuauVM, _error_handler);

    ClassDB::bind_method(D_METHOD("push_error_handler"), &LuauVM::push_error_handler);

    ClassDB::bind_method(D_METHOD("load_string", "code", "chunkname"), &LuauVM::load_string, DEFVAL("loadstring"));
    ClassDB::bind_method(D_METHOD("do_string", "code", "chunkname"), &LuauVM::do_string, DEFVAL("dostring"));

    ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuauVM::open_libraries);
    ClassDB::bind_method(D_METHOD("open_all_libraries"), &LuauVM::open_all_libraries);

    _bind_passthrough_methods();

    ADD_SIGNAL(MethodInfo("stdout", PropertyInfo(Variant::STRING, "message")));
}

static int godot_print(lua_State* L) {
    LuauVM *node = reinterpret_cast<LuauVM*>(lua_getnode(L));
    int nargs = node->lua_gettop();

    String s = String();
    for (int i = 1; i <= nargs; i++) {
        String ss;
        if (node->lua_isnumber(i) || node->lua_isstring(i))
            ss = (node->lua_tostring)(i);
        else {
            (node->lua_getglobal)("tostring"); // Push global "tostring"
            node->lua_pushvalue(i); // Push argument

            node->push_error_handler(); lua_insert(L, 1);
            int err = node->lua_pcall(1, 1, 1); // Call tostring
            node->lua_remove(1);

            if (err != LUA_OK) {
                lua_error(L);
                return 0;
            }

            ss = (node->lua_tostring)(-1);
            (node->lua_pop)(1); // Pop tostring'ed argument
        }

        s += ss;
        if (i < nargs) s += '\t';
    }

    lua_pop(L, 1); // Pop global "tostring"

    node->emit_signal("stdout", s);
    return 0;
}

void LuauVM::register_print() {
    ::lua_pushcfunction(L, godot_print, "print");
    (lua_setglobal)("print");
}


static const luaL_Reg lualibs[] = {
    {"", luaopen_base},
    {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_VECLIBNAME, luaopen_vector},
    {LUA_DBLIBNAME, luaopen_debug},
    {LUA_UTF8LIBNAME, luaopen_utf8},
    {LUA_BITLIBNAME, luaopen_bit32},
    {NULL, NULL},
};

void LuauVM::open_libraries(const Array &libraries) {
    const luaL_Reg* lib = lualibs;
    for (; lib->func; lib++)
    {
        if (!libraries.has(lib->name))
            continue;
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(lib->name);
        lua_call(1, 0);
        if (lib->name == "")
            register_print();
    }
}

void LuauVM::open_all_libraries() {
    const luaL_Reg* lib = lualibs;
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(lib->name);
        lua_call(1, 0);
    }
    register_print();
}


static int error_handler_closure(lua_State *L) {
    godot::LuauVM *node = reinterpret_cast<LuauVM*>(lua_getnode(L));
    node->call("_error_handler");
}

void LuauVM::push_error_handler() {
    lua_pushcfunction(L, error_handler_closure, NULL);
}


int64_t LuauVM::get_memory_usage_bytes() {
    return lua_gc(LUA_GCCOUNTB, 0) + 1024 * lua_gc(LUA_GCCOUNT, 0);;
}



static lua_CompileOptions luau_vm_compile_options = {
    // int optimizationLevel;
    1,

    // int debugLevel;
    1,

    // int coverageLevel;
    0,

    // const char* vectorLib;
    nullptr,

    // const char* vectorCtor;
    "vector",

    // const char** mutableGlobals;
};


bool LuauVM::load_string(const String &code, const String &chunkname) {
    auto utf8 = code.ascii();
    auto source = utf8.get_data();
    size_t bytecodeSize = 0;
    char *bytecode = luau_compile(source, strlen(source), &luau_vm_compile_options, &bytecodeSize);
    if (bytecode == nullptr)
        return -1;
    
    int status = luau_load(L, chunkname.ascii().get_data(), bytecode, bytecodeSize, 0);
    std::free(bytecode);
    return status == LUA_OK;
}

bool LuauVM::do_string(const String &code, const String &chunkname) {
    if (!load_string(code, chunkname))
        return false;
    
    push_error_handler(); lua_insert(1);
    int status = lua_pcall(0, LUA_MULTRET, 1);
    lua_remove(1);

    return status == LUA_OK;
}
