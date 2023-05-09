#include <utils.h>
#include <godot_cpp/variant/callable.hpp>


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

        case godot::Variant::Type::CALLABLE: {
            godot::Callable c = var.operator godot::Callable();
            lua_pushcallable(L, c);
            break;
        }
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

        case LUA_TFUNCTION: {
            return lua_tofunction(L, idx);
        }
    }

    return godot::Variant();
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
    lua_pushvalue(L, idx);
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        godot::Variant key = lua_tovariant(L, -2);
        godot::Variant value = lua_tovariant(L, -1);
        dict[key] = value;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
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

godot::Ref<godot::LuauFunction> lua_tofunction(lua_State *L, int idx) {
    int ref = lua_ref(L, idx);
    return memnew(godot::LuauFunction(L, ref));
}


typedef struct CallableWrapped {
    lua_State *L;

    godot::Object *object_p;
    int64_t object_id;
    const char32_t *method;
} CallableWrapped;

static int lua_pushcallable_method(lua_State *L) {
    CallableWrapped *p_callablewrapped = (CallableWrapped *)lua_touserdata(L, lua_upvalueindex(1));
    
    if (!godot::UtilityFunctions::is_instance_id_valid(p_callablewrapped->object_id)) {
        godot::UtilityFunctions::print("pooped :(");
        luaL_error(L, "attempt to call method on an invalid object");
        return 0;
    }

    godot::Object *object_p = godot::ObjectDB::get_instance(p_callablewrapped->object_id);

    if (!object_p->has_method(p_callablewrapped->method)) {
        godot::UtilityFunctions::print("pooped 2 :(");
        luaL_error(L, "attempt to call invalid external method");
        return 0;
    }

    godot::Array arguments;
    int arg_count = lua_gettop(L);
    for (int i = 1; i <= arg_count; i++) {
        godot::Variant arg = lua_tovariant(L, i);
        arguments.push_back(arg);
    }
    
    godot::Variant result = object_p->callv(p_callablewrapped->method, arguments);
    lua_pushvariant(L, result);
    return 1;
}

void lua_pushcallable(lua_State *L, const godot::Callable &callable) {
    CallableWrapped *p_callablewrapped = (CallableWrapped *)lua_newuserdata(L, sizeof(CallableWrapped));
    p_callablewrapped->L = L;
    p_callablewrapped->object_id = callable.get_object_id();
    p_callablewrapped->method = callable.get_method().c_escape().ptr();
    lua_pushcclosure(L, lua_pushcallable_method, NULL, 1);
}
