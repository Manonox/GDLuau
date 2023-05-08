#include <luau_vm.h>
#include <utils.h>

#include <cstdlib>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;


int lua_vector(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs >= 4) {
        luaL_error(L, "vector4 is not supported");
        return 0;
    }

    if (nargs == 1) {
        if (lua_isvector(L, 1)) { // Copy
            auto vec = lua_tovector(L, 1);
            lua_pushvector(L, vec[0], vec[1], vec[2]);
            return 1;
        }

        if (lua_istable(L, 1)) { // From table
            double v[3] {0.0, 0.0, 0.0};

            for (int i = 1; i <= 3; i++) {
                lua_pushinteger(L, i);
                lua_gettable(L, -1);
                int isnum;
                double x = lua_tonumberx(L, -1, &isnum);
                if (!isnum) {
                    luaL_error(L, "missing table key [%d] in first argument to 'vector'", i);
                    lua_pop(L, 1);
                    return 0;
                }
                v[i-1] = x;
                lua_pop(L, 1);
            }

            lua_pushvector(L, float(v[0]), float(v[1]), float(v[2]));
            return 1;
        }
    } else {
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        double z = 0.0;
        if (nargs >= 3)
            z = luaL_checknumber(L, 3);

        lua_pushvector(L, float(x), float(y), float(z));
        return 1;
    }
}

int lua_vector_tostring(lua_State* L) {
    const float* v = luaL_checkvector(L, 1);
    lua_pushfstring(L, "%f, %f, %f", v[0], v[1], v[2]);
    return 1;
}

int lua_vector_index(lua_State* L) {
    const float* v = luaL_checkvector(L, 1);
    int isint;
    int key = lua_tointegerx(L, 2, &isint);
    
    if (!isint) {
        luaL_error(L, "attempt to index vector with %s (1/2/3 or 'x'/'y'/'z' expected)", lua_typename(L, lua_type(L, 2)));
        return 0;
    }

    if (key < 1 || key > 3) {
        luaL_error(L, "attempt to index vector with bad integer (1/2/3 or 'x'/'y'/'z' expected)", lua_typename(L, lua_type(L, 2)));
        return 0;
    }

    lua_pushnumber(L, double(v[key-1]));
    return 1;
}

static const luaL_Reg veclib[] = {
    {"new", lua_vector},
    {NULL, NULL},
};

int luaopen_vector(lua_State* L)
{
    luaL_register(L, LUA_VECLIBNAME, veclib);

    lua_pushvector(L, 0.0, 0.0, 0.0);
    lua_setfield(L, -2, "zero");

    lua_pushvector(L, -1.0, 0.0, 0.0);
    lua_setfield(L, -2, "left");
    lua_pushvector(L, 1.0, 0.0, 0.0);
    lua_setfield(L, -2, "right");
    lua_pushvector(L, 0.0, 1.0, 0.0);
    lua_setfield(L, -2, "up");
    lua_pushvector(L, 0.0, -1.0, 0.0);
    lua_setfield(L, -2, "down");
    lua_pushvector(L, 0.0, 0.0, -1.0);
    lua_setfield(L, -2, "forward");
    lua_pushvector(L, 0.0, 0.0, 1.0);
    lua_setfield(L, -2, "back");

    lua_pushvector(L, 0.0f, 0.0f, 0.0f);
    luaL_newmetatable(L, "vector");

    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, lua_vector_tostring, nullptr);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, lua_vector_index, nullptr);
    lua_settable(L, -3);

    lua_setmetatable(L, -2);
    lua_pop(L, 1);

    return 1;
}
