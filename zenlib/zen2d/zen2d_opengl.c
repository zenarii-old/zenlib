internal void
Zen2DLoadAllOpenGLFunctions(void) {
#define OPENGLPROC(function, type) gl##function = (PFNGL##type##PROC)Platform->OpenGLLoadProcedure("gl" #function);
#include "zen2d_opengl_proc_list.inc"
}

internal void
Zen2DInit(memory_arena * Arena) {
    Zen2DLoadAllOpenGLFunctions();
}

internal void
Zen2DBeginFrame() { }

internal void
Zen2DEndFrame() { }