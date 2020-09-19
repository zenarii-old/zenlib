#ifndef ZEN2D_OPENGL_H
#define ZEN2D_OPENGL_H

#ifdef BUILD_LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#define OPENGLPROC(function, type) global PFNGL##type##PROC gl##function;
#include "zen2d_opengl_proc_list.inc"

struct zen2d {
    struct rect_data {
        int x;
    } RectData;
};

// TODO(Abi): Maybe need to set the context???
internal void Zen2DLoadAllOpenGLFunctions(void);

#endif //ZEN2D_OPENGL_H
