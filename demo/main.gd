extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	print("================")

	vm.open_all_libraries()
	
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
