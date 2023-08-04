#include <classes/luau_function.h>
#include <classes/luau_function_result.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <utils.h>
#include <classes/luau_vm.h>



godot::Ref<godot::LuauFunction> lua_tofunction(lua_State *L, int idx) {
    int ref = lua_ref(L, idx);
    return memnew(godot::LuauFunction(L, ref));
}

using namespace godot;


LuauFunction::LuauFunction(lua_State *L, int ref) {
    this->L = L;
    this->ref = ref;
}

LuauFunction::~LuauFunction() {
    lua_unref(L, this->ref);
}

Ref<LuauFunctionResult> LuauFunction::pcall(const Variant **args, GDExtensionInt nargs, GDExtensionCallError &error) {
    lua_getref(L, ref); // Push lua function
    for (int i = 0; i < nargs; i++) {
        Variant arg = (*args)[i];
        lua_pushvariant(L, arg); // Push arguments
    }
    
    return pcall_internal(nargs);
}

Ref<LuauFunctionResult> LuauFunction::pcallv(const Array &args) {
    lua_getref(L, ref); // Push lua function
    int64_t nargs = args.size();
    for (int64_t i = 0; i < nargs; i++) {
        lua_pushvariant(L, args[i]); // Push arguments
    }

    return pcall_internal(nargs);
}

Ref<LuauFunctionResult> LuauFunction::pcall_internal(int nargs) {
    LuauVM *node = reinterpret_cast<LuauVM*>(lua_getnode(L));
    node->push_error_handler();
    lua_insert(L, 1);
    int result = lua_pcall(L, nargs, LUA_MULTRET, 1);
    lua_remove(L, 1);

    if (result != LUA_OK) {
        const char *err = lua_tostring(L, -1); // Pull error message
        lua_pop(L, 1); // Pop error message
        return memnew(LuauFunctionResult(String(err)));
    }

    Array results;
    int nresults = lua_gettop(L);
    results.resize(nresults);
    for (int i = 0; i < nresults; i++) {
        results[i] = lua_tovariant(L, i + 1);
    }
    lua_settop(L, 0); // Clear stack

    return memnew(LuauFunctionResult(results));
}


void LuauFunction::_bind_methods() {
    {
		MethodInfo mi;
		// mi.arguments.push_back(PropertyInfo(Variant::, "arguments"));
		mi.name = "pcall";
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "pcall", &LuauFunction::pcall, mi);
	}

    ClassDB::bind_method(D_METHOD("pcallv", "args"), &LuauFunction::pcallv);
}
