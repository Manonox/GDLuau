#include <utils.h>


void lua_pushvariant(lua_State *L, const godot::Variant &var) {
    switch (var.get_type())
    {
        case godot::Variant::Type::NIL:
            lua_pushnil(L);
            break;
        case godot::Variant::Type::STRING:
            lua_pushstring(L,(var.operator godot::String()).ascii().get_data());
            break;
        case godot::Variant::Type::INT:
            lua_pushinteger(L, (int64_t)var);
            break;
        case godot::Variant::Type::FLOAT:
            lua_pushnumber(L, var.operator double());
            break;
        case godot::Variant::Type::BOOL:
            lua_pushboolean(L, (bool)var);
            break;
        case godot::Variant::Type::ARRAY: {
            lua_pusharray(L, var.operator godot::Array());
            break;
        }
        case godot::Variant::Type::DICTIONARY: {
            lua_pushdictionary(L, var.operator godot::Dictionary());
            break;
        }

        case godot::Variant::Type::VECTOR2: {
            godot::Vector2 v = var.operator godot::Vector2();
            lua_pushvector(L, v.x, v.y, 0.0f);
            break;
        }

        case godot::Variant::Type::VECTOR2I: {
            godot::Vector2i v = var.operator godot::Vector2i();
            lua_pushvector(L, (float)v.x, (float)v.y, 0.0f);
            break;
        }

        case godot::Variant::Type::VECTOR3: {
            godot::Vector3 v = var.operator godot::Vector3();
            lua_pushvector(L, v.x, v.y, v.z);
            break;
        }

        case godot::Variant::Type::VECTOR3I: {
            godot::Vector3i v = var.operator godot::Vector3i();
            lua_pushvector(L, v.x, v.y, v.z);
            break;
        }

        /*
        case godot::Variant::Type::CALLABLE: {
            Callable c = var.operator Callable();
            
            lua_pushcfunction(L, callable_proxy, c.get_method());
            break;
        }
        */
    }
}

void lua_pusharray(lua_State *L, const godot::Array &array) {
    lua_createtable(L, array.size(), 0);

    for (int i = 0; i < array.size(); i++) {
        godot::Variant value = array[i];

        lua_pushinteger(L, i + 1);
        lua_pushvariant(L, value);

        lua_rawset(L, -3);
    }
}

void lua_pushdictionary(lua_State *L, const godot::Dictionary &dict) {
    lua_createtable(L, 0, dict.size());

    for(int i = 0; i < dict.size(); i++) {
        godot::Variant key = dict.keys()[i];
        godot::Variant value = dict[key];
        
        lua_pushvariant(L, key);
        lua_pushvariant(L, value);
        lua_rawset(L, -3);
    }
}


godot::Variant lua_tovariant(lua_State *L, int idx) {
    int type = lua_type(L, idx);
    
    switch (type) {
        case LUA_TNIL: {
            return godot::Variant();
        }

        case LUA_TBOOLEAN: {
            return lua_toboolean(L, idx);
        }

        case LUA_TNUMBER: {
            return lua_tonumber(L, idx);
        }

        case LUA_TVECTOR: {
            const float* vec = lua_tovector(L, idx);
            return godot::Vector3(vec[0], vec[1], vec[2]);
        }

        case LUA_TSTRING: {
            const char* s = lua_tostring(L, idx);
            return godot::String(s);
        }

        case LUA_TTABLE: {
            if (luaL_isarray(L, idx))
                return lua_toarray(L, idx);
            return lua_todictionary(L, idx);
        }

        /*
        case LUA_TFUNCTION: {

        }
        */
    }
}


godot::Array lua_toarray(lua_State *L, int idx) {
    size_t length = lua_objlen(L, idx);
    godot::Array array;
    for (int i = 1; i <= length; i++) {
        lua_rawgeti(L, idx, i);
        array.push_back(lua_tovariant(L, -1));
        lua_pop(L, 1);
    }
    return array;
}

godot::Dictionary lua_todictionary(lua_State *L, int idx) {
    godot::Dictionary dict;
    lua_pushnil(L);
    while (lua_next(L, idx) != 0) {
        godot::Variant key = lua_tovariant(L, -2);
        godot::Variant value = lua_tovariant(L, -1);
        dict[key] = value;
        lua_pop(L, 1);
    }
    return dict;
}


bool luaL_isarray(lua_State *L, int idx) {
    lua_pushvalue(L, idx);
    lua_pushnil(L);
    int has_keys = lua_next(L, -2);
    if (!has_keys) {
        lua_pop(L, 1);
        return false;
    }
    
    if (lua_type(L, -2) != LUA_TNUMBER) {
        lua_pop(L, 3);
        return false;
    }
    
    lua_pop(L, 2);

    size_t size = lua_objlen(L, -1);
    lua_pushinteger(L, (int)size);
    int has_more = lua_next(L, -2);

    if (!has_more) {
        lua_pop(L, 1);
        return true;
    }
    
    lua_pop(L, 3);
    return false;
}