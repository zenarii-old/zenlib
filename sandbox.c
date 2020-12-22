// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    i32 Pad;
    static_mesh Mesh;
};

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    f32 Vertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    f32 Colours[] = {
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
    };
    Platform->Core->Mesh = Zen3DStaticMeshFromData(sizeof(Vertices)/sizeof(f32), (v3 *)Vertices, (v4 *)Colours);
}

internal void
AppUpdate() {
#ifdef ZEN3D
    // TODO(Abi): figure out why i can't just set this once, is it bc i ditch the shsader?
    glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_RGBA]);
    f32 t = Platform->TotalTime;
    matrix4x4 View = LookAt(v3(4 * sin(t), 0.f, 4 * cos(t)), v3(0.f, 0.f, 0.f), v3(0.f, 1.f, 0.f));
    f32 Ratio = Platform->ScreenWidth/Platform->ScreenHeight;
    matrix4x4 Projection = PerspectiveMatrix(0.5f*PI, Ratio, 0.1f, 100.f);
    // TODO(Abi): Zen3DSetActiveCamera() to do this in the shader
    
    
    // TODO(Abi): this won't work for non static meshes,
    //            although ig meshes shouldn't last for a single frame so *shrug*
    Zen3DPushStaticMesh(&Platform->Core->Mesh);
    {
        GLuint ProjLoc = glGetUniformLocation(Zen3D->Shaders[0], "Projection");
        glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, &Projection.Elements[0][0]);
        
        GLuint ViewLoc = glGetUniformLocation(Zen3D->Shaders[0], "View");
        glUniformMatrix4fv(ViewLoc, 1, GL_TRUE, &View.Elements[0][0]);
    }
    
    {
        v3 p0, p1, p2, p3;
        p0 = v3(-.5f,  .5f, 0);
        p1 = v3(.5f,   .5f, 0);
        p2 = v3(.5f,  -.5f, 0);
        p3 = v3(-.5f, -.5f, 0);
        v4 Colour = v4(1.f, 0.f, 0.f, 1.f);
        Zen3DPushQuad(p0, p1, p2, p3, Colour);
    }
    
    Zen2DPushRect(v4(100, 100, 100, 100), v4(1.f, 0.f, 1.f, 1.f));
    if(ZenKeyDown(ZKEY_ESCAPE)) ZenPlatformQuit();
}

internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}