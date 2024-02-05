#include <vector_lib.h>
#include <lualib.h>


int lua_vector_lib_call(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs > 5) {
        luaL_error(L, "vector5+ are not supported");
        return 0;
    }

    if (nargs == 1) {
        lua_pushvector(L, 0.0f, 0.0f, 0.0f, 0.0f);
        return 1;
    }

    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    double z = 0.0;
    double w = 0.0;
    if (nargs >= 4)
        z = luaL_checknumber(L, 4);
    if (nargs == 5)
        w = luaL_checknumber(L, 5);

    lua_pushvector(L, float(x), float(y), float(z), float(w));
    return 1;
}

int lua_vector_tostring(lua_State* L) {
    const float* v = luaL_checkvector(L, 1);
    lua_pushfstring(L, "%f, %f, %f, %f", v[0], v[1], v[2], v[3]);
    return 1;
}

int lua_vector_index(lua_State* L) {
    const float* v = luaL_checkvector(L, 1);
    int key = luaL_checkinteger(L, 2);

    if (key < 1 || key > 4) {
        luaL_error(L, "attempt to index vector with %s (1/2/3/4 expected)", lua_typename(L, lua_type(L, 2)));
        return 0;
    }

    lua_pushnumber(L, double(v[key-1]));
    return 1;
}

static const luaL_Reg veclib[] = {
    // hmm...
    {NULL, NULL},
};

int luaopen_vector(lua_State* L) {
    luaL_register(L, LUA_VECLIBNAME, veclib);
    luaL_newmetatable(L, "vector_lib");

    lua_pushstring(L, "__call");
    lua_pushcfunction(L, lua_vector_lib_call, nullptr);
    lua_rawset(L, -3);

    lua_setmetatable(L, -2);

    lua_pushvector(L, 0.0, 0.0, 0.0, 0.0);
    lua_setfield(L, -2, "zero");
    lua_pushvector(L, -1.0, 0.0, 0.0, 0.0);
    lua_setfield(L, -2, "left");
    lua_pushvector(L, 1.0, 0.0, 0.0, 0.0);
    lua_setfield(L, -2, "right");
    lua_pushvector(L, 0.0, 1.0, 0.0, 0.0);
    lua_setfield(L, -2, "up");
    lua_pushvector(L, 0.0, -1.0, 0.0, 0.0);
    lua_setfield(L, -2, "down");
    lua_pushvector(L, 0.0, 0.0, -1.0, 0.0);
    lua_setfield(L, -2, "forward");
    lua_pushvector(L, 0.0, 0.0, 1.0, 0.0);
    lua_setfield(L, -2, "back");

    lua_pushvector(L, 0.0, 0.0, 0.0, 0.0);
    luaL_newmetatable(L, "vector");

    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, lua_vector_tostring, nullptr);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, lua_vector_index, nullptr);
    lua_rawset(L, -3);

    lua_setmetatable(L, -2);
    lua_pop(L, 1);

    return 1;
}
