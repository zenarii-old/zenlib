// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    font Font;
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
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 1.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
        0.8, 0.2f, 7.f, 1.f,
    };
    
    Platform->Core->Mesh = Zen3DStaticMeshFromData(sizeof(Vertices)/sizeof(v3), (v3 *)Vertices, (v4 *)Colours);
    
    Platform->Core->Font = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
    Zen2DSetDefaultFont(&Platform->Core->Font);
}

internal void
AppUpdate() {
    f32 t = Platform->TotalTime;
    
    local camera Camera = {
        CAMERA_MODE_LOOK_AT,
        v3(0, 0, 0),
        v3(0, 0, 0),
        0.5 * PI
    };
    
    Camera.Position = v3(4 * sin(t), sin(t) * cos(t), 4 * cos(t));
    Zen3DSetActiveCamera(&Camera);
    
    Zen3DPushStaticMesh(&Platform->Core->Mesh);
    
    Zen2DPushRect(v4(100, 100, 100, 100), v4(sin(t), 0.f, 1.f, 1.f));
    char * String = "2D/3D Test";
    f32 Width = Zen2DGetStringWidth(String, Zen2DGetDefaultFont(), 32);
    Zen2DPushText(String, v2((Platform->ScreenWidth - Width) * 0.5f, 300), 32);
    
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