#include <classes/luau_function.h>
#include <classes/luau_error.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <utils.h>


using namespace godot;

LuauFunction::LuauFunction(lua_State *L, int ref) {
    this->L = L;
    this->ref = ref;
}

LuauFunction::~LuauFunction() {
    lua_unref(L, this->ref);
}

Variant LuauFunction::pcall(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    lua_getref(L, ref); // Push lua function
    for (int i = 0; i < arg_count; i++) {
        Variant arg = (*args)[i];
        lua_pushvariant(L, arg); // Push arguments
    }
    
    int result = lua_pcall(L, arg_count, 1, 0);
    if (result != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        if (err == NULL)
            err = "Unknown";
        lua_pop(L, 1);
        return memnew(LuauError(String(err)));
    }

    Variant var = lua_tovariant(L, -1);
    lua_pop(L, 1);
    return var;
}

Variant LuauFunction::pcallv(const Array &arguments) {
    lua_getref(L, ref); // Push lua function
    int64_t arg_count = arguments.size();
    for (int64_t i = 0; i < arg_count; i++) {
        lua_pushvariant(L, arguments[i]); // Push arguments
    }

    int result = lua_pcall(L, arg_count, 1, 0);
    if (result != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        if (err == NULL)
            err = "Unknown";
        lua_pop(L, 1);
        return memnew(LuauError(String(err)));
    }

    Variant var = lua_tovariant(L, -1);
    lua_pop(L, 1);
    return var;
}

void LuauFunction::_bind_methods() {
    {
		MethodInfo mi;
		// mi.arguments.push_back(PropertyInfo(Variant::, "arguments"));
		mi.name = "pcall";
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "pcall", &LuauFunction::pcall, mi);
	}

    ClassDB::bind_method(D_METHOD("pcallv", "arguments"), &LuauFunction::pcallv);
}
