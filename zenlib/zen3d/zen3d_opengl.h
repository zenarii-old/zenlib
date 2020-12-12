// NOTE(Abi) Zen2d loads these already
#if defined(BUILD_LINUX) && !defined(ZEN2D)
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#define OPENGLPROC(function, type) global PFNGL##type##PROC gl##function;
#include "zen2d_opengl_proc_list.inc"

internal void Zen3DOpenGLLoadAllFunctions(void);
#endif

typedef enum zen3d_request_type zen3d_request_type;
enum zen3d_request_type {
    ZEN3D_REQUEST_STANDARD,
    
    ZEN3D_REQUEST_COUNT
};

struct zen3d_request {
    zen3d_request_type Type;
    
    struct {
        GLuint VAO, VBO;
        u32 Stride;
        u32 Size;
        u32 Max;
        unsigned char * Memory;
        u32 AllocPos;
    } Mesh;
};

struct zen3d {
    u32 Shader;
};

typedef struct mesh mesh;
struct mesh {
    v3 * Vertices;
    v4 * Colours;
};


