extends LuauVM


func _ready():
	pass

func _on_stdout(message):
	print("VM:", message)
