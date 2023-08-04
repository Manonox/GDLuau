#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <classes/luau_function_result.h>
#include <lua.h>
#include <lualib.h>




namespace godot {

class LuauFunction : public RefCounted {
    GDCLASS(LuauFunction, RefCounted)

private:
    lua_State *L = nullptr;
    int ref = LUA_REFNIL;

protected:
    static void _bind_methods();

public:
    LuauFunction() {}
    LuauFunction(lua_State *L, int ref);
    ~LuauFunction();

    Ref<LuauFunctionResult> pcall(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
    Ref<LuauFunctionResult> pcallv(const Array &arguments);

private:
    Ref<LuauFunctionResult> pcall_internal(int arg_count);
};

}


godot::Ref<godot::LuauFunction> lua_tofunction(lua_State *L, int idx);
