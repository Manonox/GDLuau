#include <lua_vm.h>
#include <utils.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>


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
    ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuauVM::open_libraries);
    ClassDB::bind_method(D_METHOD("open_all_libraries"), &LuauVM::open_all_libraries);

    ClassDB::bind_method(D_METHOD("pushvariant", "var"), &LuauVM::pushvariant);
    ClassDB::bind_method(D_METHOD("tovariant", "index"), &LuauVM::tovariant);
    
    _bind_passthrough_methods();
}

static int lua_print(lua_State* L) {
    int n = lua_gettop(L); /* number of arguments */
    String s = String();
    for (int i = 1; i <= n; i++) {
        const char* ss = lua_tostring(L, i);
        s += String(ss);
    }
    UtilityFunctions::print(s);
    return 0;
}

void LuauVM::register_lua_print() {
    lua_pushcfunction(L, lua_print, "print");
    lua_setglobal(L, "print");
}

LuauVM::LuauVM() {
    L = lua_newstate(lua_alloc, nullptr);
    register_lua_print();
}

LuauVM::~LuauVM() {
    lua_close(L);
}

static const luaL_Reg lualibs[] = {
    {"", luaopen_base},
    {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_DBLIBNAME, luaopen_debug},
    {LUA_UTF8LIBNAME, luaopen_utf8},
    {LUA_BITLIBNAME, luaopen_bit32},
    {NULL, NULL},
};

void LuauVM::open_libraries(Array libraries) {
    const luaL_Reg* lib = lualibs;
    for (; lib->func; lib++)
    {
        if (!libraries.has(lib->name))
            continue;
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
        if (lib->name == "")
            register_lua_print();
    }
}

void LuauVM::open_all_libraries() {
    const luaL_Reg* lib = lualibs;
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func, NULL);
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
    }
    register_lua_print();
}


void LuauVM::pushvariant(Variant var) {
    switch (var.get_type())
    {
        case Variant::Type::NIL:
            lua_pushnil(L);
            break;
        case Variant::Type::STRING:
            lua_pushstring(L,(var.operator String()).ascii().get_data());
            break;
        case Variant::Type::INT:
            lua_pushinteger(L, (int64_t)var);
            break;
        case Variant::Type::FLOAT:
            lua_pushnumber(L, var.operator double());
            break;
        case Variant::Type::BOOL:
            lua_pushboolean(L, (bool)var);
            break;

        case Variant::Type::ARRAY: {
            Array array = var.operator Array();
            lua_createtable(L, array.size(), 0);

            for (int i = 0; i < array.size(); i++) {
                Variant value = array[i];

                lua_pushinteger(L, i + 1);
                pushvariant(value);

                lua_rawset(L, -3);
            }
            break;
        }

        case Variant::Type::DICTIONARY: {
            Dictionary dict = var.operator Dictionary();
            lua_createtable(L, 0, dict.size());

            for(int i = 0; i < dict.size(); i++) {
                Variant key = dict.keys()[i];
                Variant value = dict[key];
                
                pushvariant(key);
                pushvariant(value);
                lua_rawset(L, -3);
            }
            break;
        }


        case Variant::Type::VECTOR2: {
            Vector2 v = var.operator Vector2();
            lua_pushvector(L, v.x, v.y, 0.0f);
            break;
        }

        case Variant::Type::VECTOR2I: {
            Vector2i v = var.operator Vector2i();
            lua_pushvector(L, (float)v.x, (float)v.y, 0.0f);
            break;
        }

        case Variant::Type::VECTOR3: {
            Vector3 v = var.operator Vector3();
            lua_pushvector(L, v.x, v.y, v.z);
            break;
        }

        case Variant::Type::VECTOR3I: {
            Vector3i v = var.operator Vector3i();
            lua_pushvector(L, v.x, v.y, v.z);
            break;
        }

        /*
        case Variant::Type::CALLABLE: {
            Callable c = var.operator Callable();
            
            lua_pushcfunction(L, callable_proxy, c.get_method());
            break;
        }
        */
    }
}

Variant LuauVM::tovariant(int index) {
    int type = lua_type(L, index);
    
    switch (type) {
        case LUA_TNIL: {
            return Variant();
        }

        case LUA_TBOOLEAN: {
            return lua_toboolean(L, index);
        }

        case LUA_TNUMBER: {
            return lua_tonumber(L, index);
        }

        case LUA_TVECTOR: {
            const float* vec = lua_tovector(L, index);
            
            /*
            StkId o = index2addr(L, index);
            if (!ttisvector(o))
                return NULL;
            return vvalue(o);
            */
            
            return Vector3(vec[0], vec[1], vec[2]);
        }

        case LUA_TSTRING: {
            const char* s = lua_tostring(L, index);
            return String(s);
        }

        case LUA_TTABLE: {
            bool isarray = luaL_isarray(L, index);

            if (isarray) {
                size_t length = lua_objlen(L, index);
                Array array;
                for (int i = 1; i <= length; i++) {
                    lua_rawgeti(L, index, i);
                    array.push_back(tovariant(-1));
                    lua_pop(L, 1);
                }
                return array;
            }

            Dictionary dict;
            lua_pushnil(L);
            // Stranniy kostil
            while (lua_next(L, index) != 0) { // (index < 0) ? (index - 1) : (index)
                Variant key = tovariant(-2);
                Variant value = tovariant(-1);
                dict[key] = value;
                lua_pop(L, 1);
            }
            return Variant(dict);
        }

        /*
        case LUA_TFUNCTION: {

        }
        */
    }
}

int LuauVM::do_string(String s) {
    auto utf8 = s.ascii();
    auto source = utf8.get_data();
    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(source, strlen(source), NULL, &bytecodeSize);
    int result = luau_load(L, "fuck", bytecode, bytecodeSize, 0);
    std::free(bytecode);
    if (result != 0)
        return -1;
    return lua_pcall(L, 0, 0, 0);
}
