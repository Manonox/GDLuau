#include <classes/luau_function_result.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>


using namespace godot;

LuauFunctionResult::LuauFunctionResult(const Array &tuple) {
    this->tuple = tuple;
    this->_is_error = false;
}

LuauFunctionResult::LuauFunctionResult(const String &error) {
    this->error = error;
    this->_is_error = true;
}

Array LuauFunctionResult::get_tuple() {
    if (_is_error) return Variant();
    return this->tuple;
}

String LuauFunctionResult::get_error() {
    if (!_is_error) return Variant();
    return this->error;
}

bool LuauFunctionResult::is_error() {
    return this->_is_error;
}


void LuauFunctionResult::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_error"), &LuauFunctionResult::get_error);
    ClassDB::bind_method(D_METHOD("get_tuple"), &LuauFunctionResult::get_tuple);
}
