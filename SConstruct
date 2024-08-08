#!/usr/bin/env python
import os


def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)


def validate_parent_dir(key, val, env):
    if not os.path.isdir(normalize_path(os.path.dirname(val), env)):
        raise UserError("'%s' is not a directory: %s" % (key, os.path.dirname(val)))


libname = "gdluau"
projectdir = "demo"

localEnv = Environment(tools=["default"], PLATFORM="")

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Add(
    BoolVariable(
        key="compiledb",
        help="Generate compilation DB (`compile_commands.json`) for external tools",
        default=localEnv.get("compiledb", False),
    )
)
opts.Add(
    PathVariable(
        key="compiledb_file",
        help="Path to a custom `compile_commands.json` file",
        default=localEnv.get("compiledb_file", "compile_commands.json"),
        validator=validate_parent_dir,
    )
)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()
env["compiledb"] = False

env.Tool("compilation_db")
compilation_db = env.CompilationDatabase(
    normalize_path(localEnv["compiledb_file"], localEnv)
)
env.Alias("compiledb", compilation_db)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=["src/", "/"])
sources = Glob("src/*.cpp")
sources.append(Glob("src/classes/*.cpp"))

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])



### > LUAU STUFF

# lua_env = env.Clone()

# # if env["target"] == "template_debug":
# #     lua_env.Append(CCFLAGS=["-g"])

# if lua_env["platform"] == "linux":
#     lua_env.Append(CPPDEFINES=["LUA_USE_POSIX"])
# elif lua_env["platform"] == "ios":
#     lua_env.Append(CPPDEFINES=["LUA_USE_IOS"])

# lua_env.Append(CPPDEFINES = ["MAKE_LIB"])
# # lua_env.Append(CXXFLAGS = ["-std=c++17"])
# lua_env.Append(CFLAGS = ["-std=c99"])

lua_cpp_paths = []

luau_paths = [
    "Common",
    "Ast",
    "Compiler",
    "CodeGen",
    "Analysis",
    "Config",
    "VM",
]

luau_include_paths = [os.path.join("luau", x, "include") for x in luau_paths]
luau_source_paths = [os.path.join("luau", x, "src") for x in luau_paths]
lua_cpp_paths.extend(luau_include_paths)
lua_cpp_paths.extend(luau_source_paths)

env.Append(CPPPATH=lua_cpp_paths)
# env.AppendUnique(CPPPATH=lua_cpp_paths, delete_existing=True)


lua_includes = []
lua_sources = []
for path in luau_include_paths:
    lua_includes.extend(Glob(path + "/*.hpp"))
    lua_includes.extend(Glob(path + "/*.h"))
    # lua_includes.extend(Glob(path + "/*.cpp"))
    # lua_includes.extend(Glob(path + "/*.c"))

for path in luau_source_paths:
    # lua_sources.extend(Glob(path + "/*.hpp"))
    # lua_sources.extend(Glob(path + "/*.h"))
    lua_sources.extend(Glob(path + "/*.cpp"))
    lua_sources.extend(Glob(path + "/*.c"))

sources.extend(lua_sources)

# lua_file = "{}{}{}".format("luau", env["suffix"], env["SHLIBSUFFIX"])
# lua_libraryfile = "bin/{}/{}".format(env["platform"], lua_file)
# lua_library = lua_env.StaticLibrary(lua_libraryfile, source=lua_sources)

### < LUAU STUFF




if env["platform"] == "macos":
    platlibname = "{}.{}.{}".format(libname, env["platform"], env["target"])
    file = "{}.framework/{}".format(env["platform"], platlibname, platlibname)

libraryfile = "bin/{}/{}".format(env["platform"], file)

# env.Append(CXXFLAGS=["-g"])
env.Append(CXXFLAGS=["-fexceptions"])
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

copy = env.InstallAs("{}/addons/gdluau/bin/{}/{}".format(projectdir, env["platform"], file), library)

default_args = [library, copy]
if localEnv.get("compiledb", False):
    default_args += [compilation_db]
Default(*default_args)
