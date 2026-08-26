#!/usr/bin/env python
import os

from SCons.Errors import UserError


def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)


def validate_parent_dir(key, val, env):
    if not os.path.isdir(normalize_path(os.path.dirname(val), env)):
        raise UserError("'%s' is not a directory: %s" % (key, os.path.dirname(val)))

bindir = "bin"
build_dir = os.path.join(bindir, "build")
# Put object files under bin/build/ instead of beside sources
VariantDir(build_dir, ".", duplicate=0)
VariantDir(os.path.join(build_dir, "luau"), "luau", duplicate=0)

libname = "gdluau"
projectdir = "demo/addons/gdluau"

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

env = SConscript(
    "godot-cpp/SConstruct",
    {"env": env, "customs": customs, "api_version": "4.7"},
)

lua_env = env.Clone()
module_env = env.Clone()

if lua_env["platform"] == "linux":
    lua_env.Append(CPPDEFINES=["LUA_USE_POSIX"])
elif lua_env["platform"] == "ios":
    lua_env.Append(CPPDEFINES=["LUA_USE_IOS"])

lua_env.Append(CPPDEFINES=["MAKE_LIB"])
lua_env.Append(CXXFLAGS=["-std=c++17"])
lua_env.Append(CFLAGS=["-std=c99"])

luau_paths = [
    "Common",
    "Ast",
    "Bytecode",
    "Compiler",
    "CodeGen",
    "Analysis",
    "Config",
    "VM",
]

luau_include_paths = [os.path.join("luau", x, "include") for x in luau_paths]
luau_source_paths = [os.path.join("luau", x, "src") for x in luau_paths]

lua_cpp_paths = luau_include_paths + luau_source_paths
cpp_paths = [Dir("src").abspath, Dir("#").abspath] + luau_include_paths

lua_env.AppendUnique(CPPPATH=lua_cpp_paths, delete_existing=True)
module_env.Append(CPPPATH=cpp_paths)

lua_sources = []
for path in luau_source_paths:
    lua_sources.extend(Glob(path + "/*.cpp"))
    lua_sources.extend(Glob(path + "/*.c"))

luau_library_name = "gdluau{}{}".format(env["suffix"], env["LIBSUFFIX"])
luau_library = lua_env.StaticLibrary("bin/{}".format(luau_library_name), source=lua_sources)

module_env.Append(LIBPATH=[Dir("bin").abspath])
module_env.Append(LIBS=[luau_library_name])

sources = Glob("*.cpp")
sources.extend(Glob("src/*.cpp"))
sources.extend(Glob("src/classes/*.cpp"))

if env["platform"] == "macos":
    library = module_env.SharedLibrary(
        "{}/bin/macos/macos.framework/lib{}.{}.{}".format(
            projectdir, libname, env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = module_env.StaticLibrary(
            "{}/bin/lib{}.{}.{}.simulator.a".format(
                projectdir, libname, env["platform"], env["target"]
            ),
            source=sources,
        )
    else:
        library = module_env.StaticLibrary(
            "{}/bin/lib{}.{}.a".format(projectdir, libname, env["platform"], env["target"]),
            source=sources,
        )
else:
    library = module_env.SharedLibrary(
        "{}/bin/{}/{}{}{}".format(
            projectdir, env["platform"], libname, env["suffix"], env["SHLIBSUFFIX"]
        ),
        source=sources,
    )

default_args = [luau_library, library]
if localEnv.get("compiledb", False):
    default_args += [compilation_db]
Default(*default_args)
