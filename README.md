# GDLuau
A GDExtension that adds [Luau](https://luau-lang.org) C/C++ API bindings to GDScript
*(+ some extras)*

About
-----
GDLuau is for in-game scripting / UGC / modding.
*(This is **not** a replacement for or an alternative to GDScript.)*

Luau API closely follows Lua 5.1 API, you can find the manual [here](https://www.lua.org/manual/5.1/manual.html).<br />
For coding in lua/luau take a look at the ["Getting Started" section](https://luau-lang.org/getting-started) on [luau-lang.org](https://luau-lang.org).<br />


Features
--------
- Full C/C++ API excluding userdata
- Pushing/Pulling normal Callables (lambda Callables aren't supported)
- Pushing/Pulling Objects (Node / RefCounted) (as userdata)
- Pushing/Pulling Godot Arrays and Dictionaries
- Pushing/Pulling Godot Vector3s as Luau vectors
- Custom print implementation that emits a signal
- lua_loadstring, lua_dostring in GDScript (were "missing" in Luau)


Compiling
------------
- Clone the repo
- Run `scons -j=4 platform=<platform>`
- [A test project is included in the repo](https://github.com/Manonox/GDLuau/tree/master/project)
  (binaries are in `res://addons/gdluau/bin`)

---------------
[![lua logo](https://luau-lang.org/assets/images/luau.png)](https://luau-lang.org)<br />