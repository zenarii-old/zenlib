#ifndef ZEN2D_OPENGL_H
#define ZEN2D_OPENGL_H

#ifdef BUILD_LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#define OPENGLPROC(function, type) global PFNGL##type##PROC gl##function;
#include "zen2d_opengl_proc_list.inc"

// NOTE(Abi): Shaders
typedef struct zen2d_shader_info zen2d_shader_info;
struct zen2d_shader_info {
    char * Name;
    char * VertexSource;
    char * FragmentSource;
};

enum zen2d_shader_type {
    ZEN2D_SHADER_RECTANGLES,
    
    ZEN2D_SHADER_COUNT
};

struct zen2d {
    ZEN2D_COMMON;
    
    u32 Shaders[ZEN2D_SHADER_COUNT];
    
    u32 GeneralVAO;
    
#define ZEN2DBATCHTYPE(lower_name, upper_name, stride, max) \
struct zen2d_##lower_name##_data { \
u32 VAO, VBO; \
unsigned char * Memory; \
u32 AllocPos; \
u32 Stride; \
u32 Size; \
} upper_name; 
#include "zen2d_batch_data_types.inc"
};

// NOTE(Abi): OpenGL Specific Shaders
internal void Zen2DLoadAllOpenGLFunctions(void);

#endif //ZEN2D_OPENGL_H
