#include <classes/luau_function.h>



using namespace godot;


static LuauFunction lua_tofunction(lua_State *L, int index) {
    lua_pushvalue(L, index);
    int ref = lua_ref(L, LUA_REGISTRYINDEX);
    return LuauFunction(L, ref);
}

LuauFunction::LuauFunction(lua_State *L, int ref) {
    this->L = L;
    this->ref = ref;
}

LuauFunction::~LuauFunction() {
    lua_unref(L, ref);
}

Variant LuauFunction::call(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    
}

void LuauFunction::_bind_methods() {
    {
		MethodInfo mi;
		mi.arguments.push_back(PropertyInfo(Variant::STRING, "arguments"));
		mi.name = "call";
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "call", &LuauFunction::call, mi);
	}
}
