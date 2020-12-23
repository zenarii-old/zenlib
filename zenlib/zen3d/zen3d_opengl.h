// TODO(Abi):
// Zen3DPushLine() Can also then be wrapped
// Zen3DPushShape() Quads, Cube maybe idk
// Zen3DPushMesh() mesh can use a custom shader
// Zen3DPushStaticMesh() //only needs their VAOs

// Zen3DPushMode() wireframe and things
// Ability to enable and disable depth etc

// use a Uniform Buffer Object for things like perspective/view.
// model matrices should be optional for some shaders i guess

// Zen3DPushShader(shader s) { Zen3D->ActiveCustomShader = s };
// is a type of request so change in end frame too
// when pushing meshes then use glUseProgram(Zen3D->ActiveCustomShader)
// use a similar push camera
typedef struct zen3d_shader_info zen3d_shader_info;
struct zen3d_shader_info {
    char * Name;
    char * VertexSource;
    char * FragmentSource;
};

typedef enum zen3d_shader_type zen3d_shader_type;
enum zen3d_shader_type {
    ZEN3D_SHADER_RGBA,
    
    ZEN3D_SHADER_COUNT,
};


typedef enum zen3d_request_type zen3d_request_type;
enum zen3d_request_type {
    ZEN3D_REQUEST_RGBA,
    ZEN3D_REQUEST_STATIC_MESH,
    ZEN3D_REQUEST_SET_CAMERA,
    
    ZEN3D_REQUEST_COUNT
};

typedef struct zen3d_request zen3d_request;
struct zen3d_request {
    zen3d_request_type Type;
    
    void * Data;
    u32 DataLength;
};

struct zen3d {
    ZEN3D_COMMON;
    
    GLuint Shaders[ZEN3D_SHADER_COUNT];
    GLuint GeneralVAO;
    
    struct { \
        GLuint VAO, VBO; \
        u32 Stride; \
        u32 Size; \
        u32 Max; \
        unsigned char * Memory; \
        u32 AllocPos; \
    } Shapes;
    
#define ZEN3D_MAX_REQUESTS 2048
    zen3d_request * ActiveRequest;
    zen3d_request   Requests[ZEN3D_MAX_REQUESTS];
    u32 RequestCount;
    
    framebuffer Framebuffer;
};


struct static_mesh {
    u32 VAO, VBO;
    u32 VerticesCount;
};

typedef enum camera_mode camera_mode;
enum camera_mode {
    CAMERA_MODE_LOOK_AT,
    CAMERA_MODE_COUNT
};

struct camera {
    camera_mode Mode;
    v3 Position;
    v3 Target;
    f32 fov;
};