mkdir -p build
gcc -Werror -Wall -Wno-unused-function -DINTERNAL_BUILD -DBUILD_LINUX zenlib/zencore/zencore_linux.c -o build/out -lX11 -lGL -lGLU -lm -ludev
date +"%D %T"