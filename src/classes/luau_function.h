#ifndef LUAU_FUNCTION_H
#define LUAU_FUNCTION_H

#include <godot_cpp/classes/ref_counted.hpp>
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

    Variant pcall(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
};

}

#endif
