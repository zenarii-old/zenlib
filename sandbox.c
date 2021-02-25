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
    
    v3 Vertices[] = {
        v3(-1.0f,-1.0f,-1.0f),
        v3(-1.0f,-1.0f, 1.0f),
        v3(-1.0f, 1.0f, 1.0f),
        v3(1.0f, 1.0f,-1.0f),
        v3(-1.0f,-1.0f,-1.0f),
        v3(-1.0f, 1.0f,-1.0f),
        v3(1.0f,-1.0f, 1.0f),
        v3(-1.0f,-1.0f,-1.0f),
        v3(1.0f,-1.0f,-1.0f),
        v3(1.0f, 1.0f,-1.0f),
        v3(1.0f,-1.0f,-1.0f),
        v3(-1.0f,-1.0f,-1.0f),
        v3(-1.0f,-1.0f,-1.0f),
        v3(-1.0f, 1.0f, 1.0f),
        v3(-1.0f, 1.0f,-1.0f),
        v3(1.0f,-1.0f, 1.0f),
        v3(-1.0f,-1.0f, 1.0f),
        v3(-1.0f,-1.0f,-1.0f),
        v3(-1.0f, 1.0f, 1.0f),
        v3(-1.0f,-1.0f, 1.0f),
        v3(1.0f,-1.0f, 1.0f),
        v3(1.0f, 1.0f, 1.0f),
        v3(1.0f,-1.0f,-1.0f),
        v3(1.0f, 1.0f,-1.0f),
        v3(1.0f,-1.0f,-1.0f),
        v3(1.0f, 1.0f, 1.0f),
        v3(1.0f,-1.0f, 1.0f),
        v3(1.0f, 1.0f, 1.0f),
        v3(1.0f, 1.0f,-1.0f),
        v3(-1.0f, 1.0f,-1.0f),
        v3(1.0f, 1.0f, 1.0f),
        v3(-1.0f, 1.0f,-1.0f),
        v3(-1.0f, 1.0f, 1.0f),
        v3(1.0f, 1.0f, 1.0f),
        v3(-1.0f, 1.0f, 1.0f),
        v3(1.0f,-1.0f, 1.0f)
    };
    
    
    v4 Colours[sizeof(Vertices)/sizeof(v3)] = {0};
    for(int i = 0; i < sizeof(Vertices)/sizeof(v3); ++i) {
        Colours[i] = v4(1.f, 0.5f, 0.31f, 1.f);
    }
    
    
    v3 Normals[sizeof(Vertices)/sizeof(v3)] = {0};
    
    for(int i = 0; i < sizeof(Vertices)/sizeof(v3); i += 3) {
        v3 Edge1 = SubtractV3(Vertices[i], Vertices[i + 1]);
        v3 Edge2 = SubtractV3(Vertices[i], Vertices[i + 2]);
        v3 Normal = CrossV3(Edge1, Edge2);
        
        Normals[i]     = Normal;
        Normals[i + 1] = Normal;
        Normals[i + 2] = Normal;
    }
    
    v2 UVs[sizeof(Vertices)/sizeof(v3)] = {0};
    
    Zen3DStaticMeshFromData(sizeof(Vertices)/sizeof(v3), Vertices, Colours, Normals, UVs);
    Platform->Core->Font = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
    Zen2DSetDefaultFont(&Platform->Core->Font);
}

internal void
AppUpdate() {
    
    local camera Camera = {0};
    Camera.Mode = CAMERA_MODE_LOOK_AT;
    Camera.fov = 0.5 * PI;
    
    // TODO(abi): figure out why these initialisers are no longer constant
    local sun Sun = {0};
    Sun.Direction = v3(1.f, 1.f, 0.f);
    Sun.Colour = v4(1.f, 1.f, 1.f, 1.f);
    
    Zen3DSetActiveSun(&Sun);
    
    local f32 t;
    if(Platform->KeyDown[KEY_A]) t += Platform->Delta * 2.f;
    if(Platform->KeyDown[KEY_D]) t -= Platform->Delta * 2.f;
    Camera.Position = v3(4 * sin(t), 0.f, 4 * cos(t));
    Zen3DSetActiveCamera(&Camera);
    
    Zen3DPushStaticMesh(&Platform->Core->Mesh);
    
    char * String = "Lighting test";
    Zen2DPushText(String, v2(10, 10), 16);
    if(ZenKeyDown(KEY_ESCAPE)) ZenPlatformQuit();
}

internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}