// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    font Font;
    texture Texture;
};

internal void
AppInit() {
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    Platform->Core->Font = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
    Zen2DSetDefaultFont(&Platform->Core->Font);
    
    Platform->Core->Texture = ZenLoadTextureFromPNG("white.png", ZEN_TEXTURE_NEAREST);
}

internal void
AppUpdate() {
#if 0
    local camera Camera = {0};
    Camera.Mode = CAMERA_MODE_LOOK_AT;
    Camera.fov = 0.5 * PI;
    
    // TODO(abi): figure out why these initialisers are no longer constant
    local sun Sun = {0};
    Sun.Direction = v3(1.f, 1.f, 0.f);
    Sun.Colour = v4(1.f, 1.f, 1.f, 1.f);
#endif
    
    //Zen2DTest
    Zen2DPushRect(v4(100, 100, 100, 100), v4(1.f, 0.f, 0.f, 1.f));
    //Zen2DPushTextureRect(Platform->Core->Texture, v2(100, 100));
    
}

internal void
AppHotUnload() {
    
}

internal void
AppHotLoad() {
    
}