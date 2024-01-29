#include <utils.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/callable.hpp>
// #include <classes/luau_function.h>



void lua_pushvariant(lua_State *L, const godot::Variant &var) {
    switch (var.get_type())
    {
        case godot::Variant::Type::NIL:
            lua_pushnil(L);
            break;
        case godot::Variant::Type::STRING:
            lua_pushstring(L, (var.operator godot::String()).ascii().get_data());
            break;
        case godot::Variant::Type::INT:
            lua_pushinteger(L, var.operator int32_t());
            break;
        case godot::Variant::Type::FLOAT:
            lua_pushnumber(L, var.operator double());
            break;
        case godot::Variant::Type::BOOL:
            lua_pushboolean(L, var.operator bool());
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
            lua_pushcallable(L, c, godot::String());
            break;
        }

        case godot::Variant::Type::OBJECT: {
            godot::Object *o = var.operator godot::Object *();
            lua_pushobject(L, o);
            break;
        }

        default: {
            lua_pushnil(L);
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

    auto keys = dict.keys();
    for(int i = 0; i < dict.size(); i++) {
        godot::Variant key = keys[i];
        godot::Variant value = dict[key];
        
        lua_pushvariant(L, key);
        if ((key.get_type() == godot::Variant::STRING || key.get_type() == godot::Variant::STRING_NAME) && value.get_type() == godot::Variant::CALLABLE) {
            lua_pushcallable(L, value.operator godot::Callable(), key.operator godot::String());
        } else {
            lua_pushvariant(L, value);
        }
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
            return (bool)lua_toboolean(L, idx);
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

        case LUA_TUSERDATA: {
            if (lua_isobject(L, idx)) {
                return lua_toobject(L, idx);
            }

            return godot::Variant();
        }

        default: {
            return godot::Variant();
        }
    }
}


godot::Variant lua_toarray(lua_State *L, int idx) {
    if (!lua_istable(L, idx)) return godot::Variant();
    size_t length = lua_objlen(L, idx);
    godot::Array array;
    for (int i = 1; i <= length; i++) {
        lua_rawgeti(L, idx, i);
        array.push_back(lua_tovariant(L, -1));
        lua_pop(L, 1);
    }
    return array;
}

godot::Variant lua_todictionary(lua_State *L, int idx) {
    if (!lua_istable(L, idx)) return godot::Variant();
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


// Might be faulty and/or unneeded
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


typedef struct CallableWrapped {
    int64_t object_id;
    char method[257];
    char debugname[129];
} CallableWrapped;

int lua_pushcallable_method(lua_State *L) {
    CallableWrapped *p_callablewrapped = (CallableWrapped *)lua_touserdata(L, lua_upvalueindex(1));
    
    if (!godot::UtilityFunctions::is_instance_id_valid(p_callablewrapped->object_id)) {
        luaL_error(L, "attempt to call method on an invalid object");
        return 0;
    }

    godot::Object *object_p = godot::ObjectDB::get_instance(p_callablewrapped->object_id);

    if (!object_p->has_method(p_callablewrapped->method)) {
        luaL_error(L, "attempt to call nonexistent external method");
        return 0;
    }
    
    auto arr = godot::Array();
    arr.push_back(lua_getnode(L));
    int nresults = object_p->callv(p_callablewrapped->method, arr); // Work-around, can't use 'call' for some reason
    return nresults;
}

void lua_pushcallable(lua_State *L, const godot::Callable &callable, const godot::String &debugname) {
    if (callable.is_custom()) {
        godot::UtilityFunctions::push_warning("[GDLUAU] Custom Callables not implemented");
        lua_pushnil(L);
        return;
    }

    if (callable.is_null()) {
        godot::UtilityFunctions::push_warning("[GDLUAU] Trying push invalid Callable");
        lua_pushnil(L);
        return;
    }

    CallableWrapped *p_callablewrapped = (CallableWrapped *)lua_newuserdata(L, sizeof(CallableWrapped));
    p_callablewrapped->object_id = callable.get_object_id();
    
    {
        auto method = (godot::String) callable.get_method();

        const int max_length = 257;
        auto length = method.ascii().length();
        if (length > max_length) {
            length = max_length;
            godot::UtilityFunctions::push_error("Too long method name");
        }

        auto src = method.to_ascii_buffer();

        for (int i = 0; i < length; i++)
            p_callablewrapped->method[i] = src[i];
        p_callablewrapped->method[length] = '\0';
    }

    {
        const int max_length = 129;
        auto length = debugname.ascii().length();
        length = length > max_length ? max_length : length;
        auto src = debugname.to_ascii_buffer();

        for (int i = 0; i < length; i++)
            p_callablewrapped->debugname[i] = src[i];
        p_callablewrapped->debugname[length] = '\0';
    }

    lua_pushcclosure(L, lua_pushcallable_method, p_callablewrapped->debugname, 1);
}


bool luaL_hasmetatable(lua_State* L, int idx, const char* tname) {
    if (!lua_getmetatable(L, idx))
        return false;
    luaL_getmetatable(L, tname);
    bool result = lua_rawequal(L, -2, -1);
    lua_pop(L, 2);
    return result;
}


typedef struct ObjectWrapped {
    int64_t object_id;
} ObjectWrapped;

void lua_pushobject(lua_State *L, godot::Object *object) {
    int tag = 0;
    if (object->is_class("RefCounted")) {
        godot::RefCounted *refcounted = reinterpret_cast<godot::RefCounted *>(object);
        refcounted->reference();
        tag = 1;
    }
    ObjectWrapped *p_nodewrapped = (ObjectWrapped *)lua_newuserdatatagged(L, sizeof(ObjectWrapped), tag);
    p_nodewrapped->object_id = object->get_instance_id();
    luaL_getmetatable(L, "object");
    lua_setmetatable(L, -2);
}

void object_userdata_dtor(lua_State *L, void *data) {
    ObjectWrapped *p_nodewrapped = reinterpret_cast<ObjectWrapped *>(data);
    godot::Object *object = godot::ObjectDB::get_instance(p_nodewrapped->object_id);
    if (object == NULL) return;
    godot::RefCounted *refcounted = godot::Object::cast_to<godot::RefCounted>(object);
    if (refcounted == NULL) return;
    refcounted->unreference();
}

godot::Object *lua_toobject(lua_State *L, int idx) {
    ObjectWrapped *p_nodewrapped = (ObjectWrapped *)lua_touserdata(L, idx);
    if (p_nodewrapped == NULL) return NULL; // Not userdata
    if (!luaL_hasmetatable(L, idx, "object")) return NULL; // Not "Object"
    return godot::ObjectDB::get_instance(p_nodewrapped->object_id);
}

int lua_isobject(lua_State *L, int idx) {
    if (!lua_isuserdata(L, idx)) return 0; // Not userdata
    if (!luaL_hasmetatable(L, idx, "object")) return 0; // Not "Object"
    return 1;
}


bool lua_isvalidobject(lua_State *L, int idx) {
    ObjectWrapped *p_nodewrapped = (ObjectWrapped *)luaL_checkudata(L, idx, "object");
    godot::Object *object = godot::ObjectDB::get_instance(p_nodewrapped->object_id);
    return object != NULL;
}

godot::Object *luaL_checkobject(lua_State *L, int idx, bool valid) {
    ObjectWrapped *p_nodewrapped = (ObjectWrapped *)luaL_checkudata(L, idx, "object");
    godot::Object *object = godot::ObjectDB::get_instance(p_nodewrapped->object_id);
    if (valid && object == NULL)
        luaL_typeerrorL(L, idx, "object");
        return NULL;
    return object;
}


void lua_setnode(lua_State* L, godot::LuauVM* node) {
    lua_pushstring(L, GDLUAU_REGISTRY_NODE_KEY);
    lua_pushlightuserdata(L, node);
    lua_settable(L, LUA_REGISTRYINDEX);
}

godot::LuauVM* lua_getnode(lua_State* L) {
    lua_pushstring(L, GDLUAU_REGISTRY_NODE_KEY);
    lua_gettable(L, LUA_REGISTRYINDEX);
    if (!lua_islightuserdata(L, -1)) {
        lua_pop(L, 1);
        return nullptr;
    }
    void *userdata = lua_tolightuserdata(L, -1);
    lua_pop(L, 1);
    return reinterpret_cast<godot::LuauVM*>(userdata);
}
