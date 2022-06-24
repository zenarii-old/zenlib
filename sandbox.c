// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    font Font;
    texture Texture;
    texture TEMPFontTexture;
};

internal void
AppInit() {
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    Zen2DSetDefaultFont(&Platform->Core->Font);
    
    Platform->Core->Texture = ZenLoadTextureFromPNG("star.png", ZEN_TEXTURE_NEAREST);
    
    Platform->Core->Font = Zen2DLoadFont("inconsolata.ttf", 32);
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
    
    v2 Centre = v2(Platform->ScreenWidth * 0.5, Platform->ScreenHeight * 0.5);
    f32 Radius = 100;
    v2 Pos = v2(Centre.x + Radius * sin(2 * Platform->TotalTime) - 50,
                Centre.y + Radius * cos(2 * Platform->TotalTime) - 50);
    
    Zen2DPushTexture(Platform->Core->Texture, Pos);
    //Zen2DPushTexture(Platform->Core->Font.Texture, v2(200, 100));
    char * Str = "Love u <3";
    Zen2DPushTextFontColourN(Str, StringLength(Str), &Platform->Core->Font, v2(100, 100), v4(1, 1, 1, 1));
}

internal void
AppHotUnload() {
    
}

internal void
AppHotLoad() {
    
}