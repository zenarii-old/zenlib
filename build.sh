error_flags="-Werror -Wall -Wno-unused-function"
defines="-DINTERNAL_BUILD -DBUILD_LINUX"
mkdir -p build
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo [ZenBuild] Building platform layer
gcc $error_flags $defines -g zenlib/zencore/zencore_linux.c -o build/out -lX11 -lGL -lGLU -lm -ludev -ldl
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo [ZenBuild] Building game layer
gcc -fPIC -shared $error_flags -g $defines -o build/sandbox.so sandbox.c
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
date +"%D %T"