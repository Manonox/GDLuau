#include <classes/luau_error.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <utils.h>


using namespace godot;


LuauError::LuauError(String message) {
    this->message = message;
}

void LuauError::set_message(String message) {
    this->message = message;
}

String LuauError::get_message() const {
    return this->message;
}

void LuauError::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_message", "s"), &LuauError::set_message);
    ClassDB::bind_method(D_METHOD("get_message"), &LuauError::get_message);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "message"), "set_message", "get_message");
}

String LuauError::_to_string() {
    return String("LuauError[\"") + this->message + String("\"]");
}
