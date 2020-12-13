typedef struct zen3d     zen3d;
typedef struct texture3d texture3d;

global zen3d * Zen3D;

#define ZEN3D_COMMON \
struct { \
f32 RendererWidth; \
f32 RendererHeight; \
};

internal void Zen3DInit(memory_arena * Arena);
internal void Zen3DBeginFrame();
internal void Zen3DEndFrame();

internal void Zen3DPushQuad(v3 p0, v3 p1, v3 p2, v3 p3);

/*
typedef struct camera3D camera3D;
struct camera3D {
    v3 Position;
    matrix4x4 View;
};
*/

#ifdef USE_OPENGL
#include "zen3d_opengl.h"
#endif