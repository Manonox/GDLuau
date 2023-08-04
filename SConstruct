#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
lua_env = env.Clone()
module_env = env.Clone()

if env["target"] == "template_debug":
    lua_env.Append(CCFLAGS=["-g"])
    module_env.Append(CCFLAGS=["-g"])

if lua_env["platform"] == "linux":
    lua_env.Append(CPPDEFINES=["LUA_USE_POSIX"])
elif lua_env["platform"] == "ios":
    lua_env.Append(CPPDEFINES=["LUA_USE_IOS"])

lua_env.Append(CPPDEFINES = ["MAKE_LIB"])
lua_env.Append(CXXFLAGS = ["-std=c++17"])
lua_env.Append(CFLAGS = ["-std=c99"])



cpp_paths = [Dir("src").abspath, Dir("/").abspath]
lua_cpp_paths = []

luau_paths = [
    "Common",
    "Ast",
    "Compiler",
    "CodeGen",
    "Analysis",
    "VM",
    #"extern/isocline",
]

luau_include_paths = [os.path.join("luau", x, "include") for x in luau_paths]
luau_source_paths = [os.path.join("luau", x, "src") for x in luau_paths]
lua_cpp_paths.extend(luau_include_paths)
lua_cpp_paths.extend(luau_source_paths)
cpp_paths.extend(luau_include_paths)

lua_env.AppendUnique(CPPPATH=lua_cpp_paths, delete_existing=True)
module_env.Append(CPPPATH=cpp_paths)


lua_sources = []
for path in luau_include_paths:
    lua_sources.extend(Glob(path + "/*.hpp"))
    lua_sources.extend(Glob(path + "/*.h"))
    lua_sources.extend(Glob(path + "/*.cpp"))
    lua_sources.extend(Glob(path + "/*.c"))

for path in luau_source_paths:
    lua_sources.extend(Glob(path + "/*.hpp"))
    lua_sources.extend(Glob(path + "/*.h"))
    lua_sources.extend(Glob(path + "/*.cpp"))
    lua_sources.extend(Glob(path + "/*.c"))

library_name = "gdluau{}{}".format(env['suffix'], env["LIBSUFFIX"])
library = lua_env.StaticLibrary("bin/{}".format(library_name), source=lua_sources)
module_env.Default(library)

module_env.Append(LIBPATH=[Dir("bin").abspath])
module_env.Append(LIBS=[library_name])

sources = Glob("*.cpp")
sources.append(Glob("src/*.cpp"))
sources.append(Glob("src/classes/*.cpp"))


env["suffix"] = env["suffix"].replace(".dev", "").replace(".double", "").replace(".simulator", "")


if env["platform"] == "macos":
    library = module_env.SharedLibrary(
        "demo/addons/gdluau/bin/libgdluau.{}.{}.framework/libgdluau.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = module_env.SharedLibrary(
        "demo/addons/gdluau/bin/libgdluau{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

env.Default(library)
