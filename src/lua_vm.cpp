#include "luau_vm.h"
#include <cstdlib>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <lua.h>
#include <lualib.h>
#include <luacode.h>

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

void LuauVM::_bind_methods() {
    ClassDB::bind_method(D_METHOD("do_string", "code"), &LuauVM::do_string);
}

LuauVM::LuauVM() {
    L = lua_newstate(lua_alloc, nullptr);
    luaL_openlibs(L);
}

LuauVM::~LuauVM() {
    lua_close(L);
}

bool LuauVM::do_string(String s) {
    /*
    UtilityFunctions::print(String("fucked ur mom"));
    auto utf8 = s.utf8();
    auto source = utf8.get_data();
    UtilityFunctions::print(String(source));
    size_t bytecodeSize = 0;
    UtilityFunctions::print(String("Huge penis"));
    char* bytecode = luau_compile(source, strlen(source), NULL, &bytecodeSize);
    UtilityFunctions::print(String("Cock and ball"));
    UtilityFunctions::print(String(bytecode));
    int result = luau_load(L, "fuck", bytecode, bytecodeSize, 0);
    UtilityFunctions::print(String("Torture"));
    //UtilityFunctions::print(itos(result));
    memfree(bytecode);
    // memdelete(bytecode);
    UtilityFunctions::print(String("penis"));
    //return result == 0;
    */
    return false;
}