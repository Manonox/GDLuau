extends Node


@onready var vm : LuauVM = $LuauVM

func _ready():
	print("================")

	vm.open_all_libraries()
	
	vm.lua_pushdictionary({
		funny = self.funny
	})
	vm.lua_setglobal("t")

	var success := vm.lua_dostring("""
		local v = t.funny
		local funcname = debug.info(v, \"n\")
		funcname = funcname ~= \"\" and (funcname .. \" at \") or funcname
		print(tostring(v), \"-- \" .. funcname .. debug.info(v, \"s\") .. \":\" .. debug.info(v, \"l\"))
	""")
	
	if success != vm.LUA_OK:
		print(vm.lua_tostring(-1))
		vm.lua_pop()
		return


func funny() -> int:
	return 0


func _on_luau_vm_stdout(message):
	print(message)
