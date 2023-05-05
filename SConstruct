#!/usr/bin/env python
import os
import sys
import fnmatch

env = SConscript("extern/SConscript")
env.tools=['mingw']
env.Append(CCFLAGS = ["-g"])

cpp_paths = [Dir('src').abspath, Dir('extern').abspath]
luau_paths = [
    "Common",
    "Ast",
    "Compiler",
    "CodeGen",
    "Analysis",
    "VM",
    "extern/isocline",
]
for path in luau_paths:
    cpp_paths.append(Dir("extern/luau/" + path + "/include").abspath)

env.Append(CPPPATH = cpp_paths)
sources = Glob('*.cpp')
sources.append(Glob('src/*.cpp'))
sources.append(Glob('src/classes/*.cpp'))


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/addons/gdluau/bin/libgdluau.{}.{}.framework/libgdluau.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "project/addons/gdluau/bin/libgdluau{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

env.Default(library)