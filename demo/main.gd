extends Node


@onready var vm : LuauVM = $LuauVM
func _ready():
	vm.open_all_libraries()
	
	vm.stdout.connect(_stdout)
	# Puts the number on top of the stack
	vm.lua_pushnumber(3)
	# Pops the top of the stack (our number) and sets a global variable "x"
	vm.lua_setglobal("x")
	# Compiles and executes a string of lua code
	var success := vm.lua_dostring("""
		local t = {
			shit = 3,
		}
		
		print("t = ", t)
		print("t.shit = ", t.shit)
		
		y = x + t.shit
	""")

	if success != vm.LUA_OK:
		# An error happened, pop an error string off the stack and print it to godot console
		print(vm.lua_tostring(-1))
		vm.lua_pop()
		return

	# Put the value of a global variable "y" on top of the stack
	vm.lua_getglobal("y")
	# "Convert" a lua number on top of the stack to a godot float
	var result := vm.lua_tonumber(-1)
	# Clear the stack (stack only has a single number in it)
	vm.lua_pop()

	print("Result (y) = %s" % result)


func _stdout(message: String) -> void:
	print("Lua says: %s" % message)
