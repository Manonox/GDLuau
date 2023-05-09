# GDLuau
GDExtension that provides adds low-level [Luau](https://luau-lang.org) API to GDScript + some extra flare
About
-----
GDLuau is for in-game scripting / UGC / modding.
*(This is **not** a replacement for or alternative to GDScript.)*

Luau API closely follows Lua 5.1 API, you can find the manual [here](https://www.lua.org/manual/5.1/manual.html).<br />
For coding in lua/luau take a look at the ["Getting Started" section](https://luau-lang.org/getting-started) on [luau-lang.org](https://luau-lang.org).<br />



Features
--------
- [x] Run code from String
- [x] Push / Pull Variant
- [x] Push Callables / Pull Functions

TODO
-----
- [ ] idk

Compiling
------------
- Clone the repo
- Run `scons -j=4 platform=[windows/linux]`
- [A test project is included in the repo](https://github.com/Manonox/GDLuau/tree/master/project)
  (binaries are in `res://addons/gdluau/bin`)

---------------
[![lua logo](https://luau-lang.org/assets/images/luau.png)](https://luau-lang.org)<br />
