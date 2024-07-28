extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	print("================")

	vm.open_all_libraries()
	
	# You can also open libraries like this
	#vm.open_libraries([
		#vm.LUA_LIB_BASE,
		#vm.LUA_LIB_COROUTINE,
		#vm.LUA_LIB_TABLE,
		#vm.LUA_LIB_OS,
		#vm.LUA_LIB_STRING,
		#vm.LUA_LIB_MATH,
		#vm.LUA_LIB_VECTOR,
		#vm.LUA_LIB_DEBUG,
		#vm.LUA_LIB_UTF8,
		#vm.LUA_LIB_BIT32
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
