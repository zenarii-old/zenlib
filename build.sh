mkdir -p build
gcc -Werror -Wall -Wno-unused-function -D INTERNAL_BUILD zenlib/zencore/zencore_linux.c -o build/out -lX11 -lGL -lGLU -lm -ludev
date +"%D %T"