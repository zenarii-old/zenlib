// TODO(Abi): Asserts to make sure not over spilling
#ifndef ZEN2D_OPENGL_H
#define ZEN2D_OPENGL_H

#ifdef BUILD_LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#define OPENGLPROC(function, type) global PFNGL##type##PROC gl##function;
#include "zen2d_opengl_proc_list.inc"

#define ZEN2D_MAX_BATCHES 2048
typedef enum zen2d_batch_type zen2d_batch_type;
enum zen2d_batch_type {
    ZEN2D_BATCH_NULL,
    ZEN2D_BATCH_RECTS,
    ZEN2D_BATCH_LINES,
    ZEN2D_BATCH_TEXTURES,
    ZEN2D_BATCH_TEXT,
    
    ZEN2D_BATCH_COUNT
};

typedef struct zen2d_batch zen2d_batch;
struct zen2d_batch {
    zen2d_batch_type Type;
    void * Data;
    u32 DataLength;
    // TODO(Abi): void * to font/text
    union {
        struct {
            u32 ID;
        } TexData;
        struct {
            font * p;
        } FontData;
    };
};

struct texture {
    GLuint ID;
    f32 Width;
    f32 Height;
};

struct font {
    texture Texture;
    font_glyph * Glyphs;
    u32 GlyphCount;
    u32 LowestChar;
    u32 LineHeight;
    u32 Base;
    u32 Size;
};

// NOTE(Abi): Shaders
typedef struct zen2d_shader_info zen2d_shader_info;
struct zen2d_shader_info {
    char * Name;
    char * VertexSource;
    char * FragmentSource;
};

enum zen2d_shader_type {
    ZEN2D_SHADER_RECTANGLES,
    ZEN2D_SHADER_LINES,
    ZEN2D_SHADER_TEXTURES,
    ZEN2D_SHADER_TEXT,
    
    ZEN2D_SHADER_COUNT
};

enum zen2d_fbo_type {
    ZEN2D_FBO_MAIN,
    ZEN2D_FBO_EFFECTS,
    
    ZEN2D_FBO_COUNT
};

typedef struct zen2d_fbo zen2d_fbo;
struct zen2d_fbo {
    GLuint ID;
    GLuint Texture;
    GLuint Depth;
    GLint Width, Height;
};

struct zen2d {
    ZEN2D_COMMON;
    
    u32 Shaders[ZEN2D_SHADER_COUNT];
    
    u32 GeneralVAO;
    
#define ZEN2DBATCH(name, stride, size, max) \
struct { \
GLuint VAO, VBO; \
u32 Stride; \
u32 Size; \
u32 Max; \
unsigned char * Memory; \
u32 AllocPos; \
} name; 
#include "zen2d_batch_data_types.inc"
    
    zen2d_fbo Framebuffer[ZEN2D_FBO_COUNT];
};

// NOTE(Abi): OpenGL Specific Functions
internal void Zen2DOpenGLLoadAllFunctions(void);
#endif //ZEN2D_OPENGL_H
