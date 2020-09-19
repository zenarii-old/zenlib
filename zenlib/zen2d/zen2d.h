#ifndef ZEN2D_H
#define ZEN2D_H

// NOTE(Abi): defined in the relevant zen2d_<renderer>.h
typedef struct zen2d zen2d;
typedef struct texture texture;

global zen2d * Zen2D;

#define ZEN2D_COMMON \
struct { \
\
}

internal void Zen2DInit(memory_arena * Arena);

#ifdef USE_OPENGL
#include "zen2d_opengl.h"
#endif

#endif //ZEN2D_H
