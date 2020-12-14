#!/bin/bash
error_flags="-Werror -Wall -Wno-unused-function"
defines="-DINTERNAL_BUILD -DBUILD_LINUX -DUSE_OPENGL"
modules="-DZENTERMINAL -DZEN3D"
mkdir -p build
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo [ZenBuild] Generating Shaders
pushd . &> /dev/null
cd zenlib/zen2d/shaders
python generate_opengl_shaders.py
popd &> /dev/null
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo [ZenBuild] Building platform layer
gcc $error_flags $modules $defines -g zenlib/zencore/zencore_linux.c -o build/out -lX11 -lGL -lGLU -lm -ludev -ldl
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo [ZenBuild] Building game layer
gcc -fPIC -shared $error_flags -g $modules $defines -o build/sandbox.so sandbox.c -lm
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
date +"Build Complete at %T"