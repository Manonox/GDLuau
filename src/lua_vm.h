#ifndef LUAU_VM_H
#define LUAU_VM_H

#include <godot_cpp/classes/node.hpp>
#include <lua.h>
#include <lualib.h>
#include <luacode.h>

namespace godot {

class LuauVM : public Node {
    GDCLASS(LuauVM, Node)

private:
    lua_State* L;

protected:
    static void _bind_methods();

public:
    LuauVM();
    ~LuauVM();

    bool do_string(String s);
};

}

#endif