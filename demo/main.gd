extends Node


@onready var vm : LuauVM = $LuauVM
@export var sandboxed := true

func _ready():
	print("================")

	if not sandboxed:
		vm.open_all_libraries()
	else:
		vm.open_libraries([
			"", # Base
			"coroutine",
			"table",
			"string",
			"utf8",
			"math",
			# "os", # This will exclude time functions as well
			"bit32",
			"buffer",
			# "debug"
		])
	
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
