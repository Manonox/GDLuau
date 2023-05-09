#include <luau_vm.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <utils.h>


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

static void lua_setnode(lua_State* L, LuauVM* node) {
    lua_pushstring(L, LUAUGD_REGISTRY_NODE_KEY);
    lua_pushlightuserdata(L, node);
    lua_settable((L), LUA_REGISTRYINDEX);
}

static LuauVM* lua_getnode(lua_State* L) {
    lua_pushstring(L, LUAUGD_REGISTRY_NODE_KEY);
    lua_gettable(L, LUA_REGISTRYINDEX);
    if (!lua_islightuserdata(L, -1))
        return nullptr;
    return reinterpret_cast<LuauVM*>(lua_tolightuserdata(L, -1));
}

LuauVM::LuauVM() {
    L = lua_newstate(lua_alloc, nullptr);
    lua_setnode(L, this);
}

LuauVM::~LuauVM() {
    lua_close(L);
}


void LuauVM::_bind_methods() {
    ClassDB::bind_method(D_METHOD("do_string", "code", "chunkname"), &LuauVM::do_string, DEFVAL("do_string"));
    ClassDB::bind_method(D_METHOD("get_lua_error"), &LuauVM::get_lua_error);
    ClassDB::bind_method(D_METHOD("get_lua_return"), &LuauVM::get_lua_return);

    ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuauVM::open_libraries);
    ClassDB::bind_method(D_METHOD("open_all_libraries"), &LuauVM::open_all_libraries);

    ClassDB::bind_method(D_METHOD("pushvariant", "var"), &LuauVM::pushvariant);
    ClassDB::bind_method(D_METHOD("pusharray", "arr"), &LuauVM::pusharray);
    ClassDB::bind_method(D_METHOD("pushdictionary", "dict"), &LuauVM::pushdictionary);

    ClassDB::bind_method(D_METHOD("tovariant", "index"), &LuauVM::tovariant);
    ClassDB::bind_method(D_METHOD("toarray", "index"), &LuauVM::toarray);
    ClassDB::bind_method(D_METHOD("todictionary", "index"), &LuauVM::todictionary);

    ClassDB::bind_method(D_METHOD("tofunction", "index"), &LuauVM::tofunction);
    ClassDB::bind_method(D_METHOD("pushcallable", "func"), &LuauVM::pushcallable);
    ClassDB::bind_method(D_METHOD("pushfunction", "func"), &LuauVM::pushcallable);

    ClassDB::bind_method(D_METHOD("error_with_traceback", "message"), &LuauVM::error_with_traceback);

    _bind_passthrough_methods();

    ADD_SIGNAL(MethodInfo("stdout", PropertyInfo(Variant::STRING, "message")));
}

static int lua_print(lua_State* L) {
    int n = lua_gettop(L); /* number of arguments */

    String s = String();
    for (int i = 1; i <= n; i++) {
        const char *ss;
        if (lua_isnumber(L, i) || lua_isstring(L, i))
            ss = lua_tostring(L, i);
        else {
            lua_getglobal(L, "tostring");
            lua_pushvalue(L, i);
            int err = lua_pcall(L, 1, 1, 0);
            if (err != 0) {
                lua_error(L);
                return 0;
            }
            ss = lua_tostring(L, -1);
            lua_pop(L, 1);
        }
        s += String(ss);
    }

    LuauVM *luau_vm = lua_getnode(L);
    luau_vm->emit_signal("stdout", s);
    return 0;
}

void LuauVM::register_lua_print() {
    lua_pushcfunction(L, lua_print, "print");
    lua_setglobal(L, "print");
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
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
        if (lib->name == "")
            register_lua_print();
    }
}

void LuauVM::open_all_libraries() {
    const luaL_Reg* lib = lualibs;
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
    }
    register_lua_print();
}


void LuauVM::pushvariant(const Variant &var) {
    lua_pushvariant(L, var);
}

void LuauVM::pusharray(const Array &array) {
    lua_pusharray(L, array);
}

void LuauVM::pushdictionary(const Dictionary &dictionary) {
    lua_pushdictionary(L, dictionary);
}

Variant LuauVM::tovariant(int index) {
    return lua_tovariant(L, index);
}

Array LuauVM::toarray(int index) {
    return lua_toarray(L, index);
}

Dictionary LuauVM::todictionary(int index) {
    return lua_todictionary(L, index);
}

Ref<LuauFunction> LuauVM::tofunction(int index) {
    return lua_tofunction(L, index);
}

Error LuauVM::pushcallable(const Callable &callable) {
    ERR_FAIL_COND_V_MSG(callable.is_custom(), ERR_INVALID_PARAMETER, "Cannot push callable, lambda callables aren't supported.");
    ERR_FAIL_COND_V_MSG(callable.is_null(), ERR_INVALID_PARAMETER, "Cannot push callable, the provided callable is invalid.");
    lua_pushcallable(L, callable);
    return OK;
}


static lua_CompileOptions luau_vm_compile_options = {
    // 0 - no optimization
    // 1 - baseline optimization level that doesn't prevent debuggability
    // 2 - includes optimizations that harm debuggability such as inlining
    // int optimizationLevel; // default=1
    1,

    // 0 - no debugging support
    // 1 - line info & function names only; sufficient for backtraces
    // 2 - full debug info with local & upvalue names; necessary for debugger
    // int debugLevel; // default=1
    1,

    // 0 - no code coverage support
    // 1 - statement coverage
    // 2 - statement and expression coverage (verbose)
    // int coverageLevel;
    0,

    // const char* vectorLib;
    nullptr,

    // const char* vectorCtor;
    "vector",

    // const char** mutableGlobals;
};

// int luau_vm_errorhandler(lua_State *L) {
//     lua_getglobal(L, "debug");
// 	if (!lua_istable(L, -1)) {
// 		lua_pop(L, 1);
// 		return 1;
// 	}
// 	lua_getfield(L, -1, "traceback");
// 	if (!lua_isfunction(L, -1)) {
// 		lua_pop(L, 2);
// 		return 1;
// 	}
// 	lua_pushvalue(L, 1);
// 	lua_pushinteger(L, 2);
// 	lua_call(L, 2, 1);
// 	return 1;
// }

bool LuauVM::do_string(const String &code, const String &chunkname) {
    auto utf8 = code.ascii();
    auto source = utf8.get_data();
    size_t bytecodeSize = 0;
    char *bytecode = luau_compile(source, strlen(source), &luau_vm_compile_options, &bytecodeSize);
    if (bytecode == nullptr) {
        last_lua_error = "Error: bytecode compilation failed.";
        return false;
    }
    int success = luau_load(L, chunkname.ascii().get_data(), bytecode, bytecodeSize, 0);
    std::free(bytecode);

    if (success != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        if (err == NULL)
            err = "Unknown (Loading)";
        last_lua_error = err;
        lua_pop(L, 1);
        return false;
    }
    
    success = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (success != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        if (err == NULL)
            err = "Unknown (Calling)";
        last_lua_error = err;
        lua_pop(L, 1);
        return false;
    }

    last_lua_return.clear();
    int nreturned = lua_gettop(L);
    for (int i = 1; i <= nreturned; i++) {
        last_lua_return.push_back(lua_tovariant(L, i));
    }

    lua_pop(L, nreturned);

    return true;
}

String LuauVM::get_lua_error() {
    return String(last_lua_error);
}

Array LuauVM::get_lua_return() {
    return last_lua_return;
}

void LuauVM::error_with_traceback(const String &message) {
    luaL_where(L, 1);
    lua_pushstring(L, message.ascii().get_data()); // memory leak ..?
    lua_concat(L, 2);
    lua_error(L);
}
