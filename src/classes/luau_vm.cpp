#include <classes/luau_vm.h>

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

LuauVM::LuauVM() {
    L = lua_newstate(lua_alloc, nullptr);
    lua_setnode(L, this);
    create_metatables();
}

LuauVM::~LuauVM() {
    lua_close(L);
}

void LuauVM::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_string", "code", "chunkname"), &LuauVM::load_string, DEFVAL("loadstring"));
    ClassDB::bind_method(D_METHOD("do_string", "code", "chunkname"), &LuauVM::do_string, DEFVAL("dostring"));

    ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuauVM::open_libraries);
    ClassDB::bind_method(D_METHOD("open_all_libraries"), &LuauVM::open_all_libraries);

    _bind_passthrough_methods();

    ADD_SIGNAL(MethodInfo("stdout", PropertyInfo(Variant::STRING, "message")));
}

int metatable_object__eq(lua_State *L) {
    Variant var1 = lua_toobject(L, 1);
    Variant var2 = lua_toobject(L, 2);
    if (var1.get_type() != Variant::Type::OBJECT && var2.get_type() != Variant::Type::OBJECT) {
        ::lua_pushboolean(L, ::lua_equal(L, 1, 2));
        return 1;
    }
    godot::Object *object1 = var1.operator godot::Object *();
    godot::Object *object2 = var2.operator godot::Object *();
    ::lua_pushboolean(L, object1->get_instance_id() == object2->get_instance_id());
    return 1;
}

void LuauVM::create_metatables() {
    luaL_newmetatable("object");
    
    lua_setuserdatadtor(L, 1, object_userdata_dtor);

    ::lua_pushstring(L, "object");
    ::lua_rawsetfield(L, -2, "__type");

    ::lua_pushcfunction(L, metatable_object__eq, NULL);
    ::lua_rawsetfield(L, -2, "__eq");

    (lua_pop)(1);
}



static int godot_print(lua_State* L) {
    LuauVM *node = lua_getnode(L);
    int nargs = node->lua_gettop();

    String s = String();
    for (int i = 1; i <= nargs; i++) {
        String ss;
        if (node->lua_isnumber(i) || node->lua_isstring(i))
            ss = (node->lua_tostring)(i);
        else {
            (node->lua_getglobal)("tostring"); // Push global "tostring"
            node->lua_pushvalue(i); // Push argument

            int err = node->lua_pcall(1, 1, 0); // Call tostring

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

    node->emit_signal("stdout", s);
    return 0;
}


int lua_loadstring(lua_State* L)
{
    size_t l = 0;
    const char* s = luaL_checklstring(L, 1, &l);
    const char* chunkname = luaL_optstring(L, 2, s);

    lua_setsafeenv(L, LUA_ENVIRONINDEX, false);

    size_t bytecode_size = 0;
    char *bytecode = luau_compile(s, l, &luau_vm_compile_options, &bytecode_size);
    if (luau_load(L, chunkname, bytecode, bytecode_size, 0) == 0)
        return 1;

    lua_pushnil(L);
    lua_insert(L, -2); // put before error message
    return 2;          // return nil plus error message
}


int luaopen_base_luau(lua_State *L) {
    int nret = luaopen_base(L);
    ::lua_pushcfunction(L, godot_print, "print");
    ::lua_rawsetfield(L, LUA_GLOBALSINDEX, "print");
    ::lua_pushcfunction(L, lua_loadstring, "loadstring");
    ::lua_rawsetfield(L, LUA_GLOBALSINDEX, "loadstring");
    return nret;
}


static const luaL_Reg lualibs[] = {
    {"", luaopen_base_luau},
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
}


int64_t LuauVM::get_memory_usage_bytes() {
    return lua_gc(LUA_GCCOUNTB, 0) + 1024 * lua_gc(LUA_GCCOUNT, 0);
}


int LuauVM::load_string(const String &code, const String &chunkname) {
    auto utf8 = code.ascii();
    auto source = utf8.get_data();
    size_t bytecode_size = 0;
    char *bytecode = luau_compile(source, strlen(source), &luau_vm_compile_options, &bytecode_size);
    if (bytecode == nullptr)
        return -1;
    
    int status = luau_load(L, chunkname.ascii().get_data(), bytecode, bytecode_size, 0);
    std::free(bytecode);
    return status;
}

int LuauVM::do_string(const String &code, const String &chunkname) {
    int status = load_string(code, chunkname);
    if (status != LUA_OK)
        return status;
    status = lua_pcall(0, LUA_MULTRET, 0);
    return status;
}
