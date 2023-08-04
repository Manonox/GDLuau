#ifndef GDLUAU_REGISTER_TYPES_H
#define GDLUAU_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_gdluau_module(ModuleInitializationLevel p_level);
void uninitialize_gdluau_module(ModuleInitializationLevel p_level);

#endif // GDLUAU_REGISTER_TYPES_H
