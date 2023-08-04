extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	print("================")

	vm.open_all_libraries()
	
	var success := vm.load_string("""
		function fuckoff(x: number): table
			return { x = x }
		end

		local t = {}
		for i, x in {1, 2, 3} do
			t[i] = fuckoff(x)
		end

		for _, v in t do
			print(`COCK AND BALLS {v.x}`)
		end
	""")
	
	if not success:
		print(":(")
		return
	
	vm.lua_pushdictionary({ poop = 7 })
	vm.lua_pushcallable(self._magic)
	var status := vm.lua_pcall(2, 0, 0)
	if status != vm.LUA_OK:
		var error := vm.lua_tostring(-1)
		vm.lua_pop(1)
		print(error)

func _magic() -> int:
	var x := vm.luaL_checknumber(1)
	vm.lua_pushnumber(x * x - 7)
	return 1

func _on_luau_vm_stdout(message):
	print(message)
