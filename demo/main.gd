extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	vm.open_all_libraries()
	
	vm.stdout.connect(_stdout)
	
	vm.lua_pushcallable(without_arg)
	vm.lua_setglobal("without_arg")
	
	vm.lua_pushcallable(with_arg)
	vm.lua_setglobal("with_arg")
	
	vm.do_string("without_arg()")
	vm.do_string("print(with_arg())")

func without_arg() -> int:
	print("Hello from GDScript #1")
	return 0

func with_arg(_vm: LuauVM) -> int:
	print("Hello from GDScript #2")
	_vm.lua_pushstring("Hello")
	return 1

func _stdout(message: String) -> void:
	print("Lua says: %s" % message)
