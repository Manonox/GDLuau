#!/usr/bin/env python
import os
import sys
import fnmatch

env = SConscript("extern/SConscript")
env.tools=['mingw']

env.Append(CPPPATH = [Dir('src').abspath, Dir('extern').abspath])
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
        "demo/addons/gdluau/bin/libgdluau{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

env.Default(library)