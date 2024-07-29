extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	print("================")

	vm.open_all_libraries()
	
	# You can also open libraries like this
	#vm.open_libraries([
		#vm.LUA_BASE_LIB,
		#vm.LUA_COROUTINE_LIB,
		#vm.LUA_TABLE_LIB,
		#vm.LUA_OS_LIB,
		#vm.LUA_STRING_LIB,
		#vm.LUA_MATH_LIB,
		#vm.LUA_VECTOR_LIB,
		#vm.LUA_DEBUG_LIB,
		#vm.LUA_UTF8_LIB,
		#vm.LUA_BIT32_LIB
	#])
	
	vm.lua_pushobject(self)
	vm.lua_setfield(vm.LUA_GLOBALSINDEX, "node")

	var success := vm.lua_dostring("""
		node.shit = 3
	""")
	
	if success != vm.LUA_OK:
		print(vm.lua_tostring(-1))
		vm.lua_pop()
		return
	

func _on_luau_vm_stdout(message):
	print(message)
