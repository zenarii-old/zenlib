// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

/*
TODO:
 * Play sounds
* Load models + materials
* Sort out lighting, shadows, etc
* Resizing of screen
*/

struct core {
    font Font;
    texture Texture;
    texture TEMPFontTexture;
    camera Camera;
};

internal void
AppInit() {
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    core * Core = Platform->Core;
    
    Zen2DSetDefaultFont(&Platform->Core->Font);
    
    Core->Texture = ZenLoadTextureFromPNG("star.png", ZEN_TEXTURE_NEAREST);
    
    Core->Font = Zen2DLoadFont("inconsolata.ttf", 64);
    
    
    Core->Camera.Target   = v3(0, 0, 1);
    Core->Camera.Mode = CAMERA_MODE_FORWARD; // TODO(Abi): target doesn't look forward
    Core->Camera.fov = 0.5 * PI;
}

internal void
AppUpdate() {
    // TEMP(Abi): 3D
    Platform->Core->Camera.Position = v3(sin(Platform->TotalTime), -1, cos(Platform->TotalTime));
    Zen3DSetActiveCamera(&Platform->Core->Camera);
    
    Zen3DPushQuad(v3(1, -1, 2), v3(-1, -1, 2), v3(-1, 1, 2), v3(1, 1, 2), v4(0, 0, 0, 1));
    
    // TODO(abi): figure out why these initialisers are no longer constant
    local sun Sun = {0};
    Sun.Direction = v3(1.f, 1.f, 0.f);
    Sun.Colour = v4(1.f, 1.f, 1.f, 1.f);
    Zen3DSetActiveSun(&Sun);
    
    // TEMP(Abi): 2D
    
    v2 Centre = v2(Platform->ScreenWidth * 0.5, Platform->ScreenHeight * 0.5);
    f32 Radius = 150;
    v2 Pos = v2(Centre.x + Radius * sin(2 * Platform->TotalTime) - 50,
                Centre.y + Radius * cos(2 * Platform->TotalTime) - 50);
    
    Zen2DPushTexture(Platform->Core->Texture, Pos);
    //Zen2DPushTexture(Platform->Core->Texture, v2(100, 100));
    char * Str = "That kinda rude";
    Zen2DPushTextFontColourN(Str, StringLength(Str), &Platform->Core->Font, v2(100, 100), v4(1, 1, 1, 1));
}

internal void
AppHotUnload() {
    Log("Reloaded :)");
}

internal void
AppHotLoad() {
    
}