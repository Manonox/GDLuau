#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <lua.h>
#include <lualib.h>


namespace godot {

class LuauFunctionResult : public RefCounted {
    GDCLASS(LuauFunctionResult, RefCounted)

private:
    Array tuple;
    String error;
    bool _is_error = false;

protected:
    static void _bind_methods();

public:
    LuauFunctionResult() {}
    LuauFunctionResult(const Array &tuple);
    LuauFunctionResult(const String &error);
    ~LuauFunctionResult() {}

    Array get_tuple();
    String get_error();
    bool is_error();
};

}

