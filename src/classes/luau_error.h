#ifndef LUAU_ERROR_H
#define LUAU_ERROR_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <lua.h>
#include <lualib.h>


using namespace godot;

class LuauError : public RefCounted {
    GDCLASS(LuauError, RefCounted);

    protected:
        static void _bind_methods();

    public:
        LuauError() {}
        LuauError(String message);

        void set_message(String message);
        String get_message() const;

        String _to_string();

    private:
        String message;
};

#endif
