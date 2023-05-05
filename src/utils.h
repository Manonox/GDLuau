#include <lua.h>
#include <lualib.h>


static bool luaL_isarray(lua_State *L, int idx) {
    /*
    function is_array(tbl)
        local first = next(tbl)
        if type(first) ~= "number" then
            return first == nil and true or false
        end
        local size = #tbl
        local key = next(tbl, size)
        return type(key) == "nil"
    end
    */
    
    lua_pushnil(L);
    int has_keys = lua_next(L, idx);
    if (!has_keys)
        return true;
    
    if (lua_type(L, -2) != LUA_TNUMBER)
        return false;
    
    lua_pop(L, 2);

    size_t size = lua_objlen(L, idx);
    lua_pushinteger(L, (int)size);
    int has_more = lua_next(L, idx);

    if (!has_more)
        return true;
    
    lua_pop(L, 2);
    return false;
}