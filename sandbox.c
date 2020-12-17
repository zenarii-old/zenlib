// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    i32 Pad;
    //font Font;
};

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    //Zen2DSetDefaultFont(&Platform->Core->Font);
    //Platform->Core->Font = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
}

internal void
AppUpdate() {
    {
        v3 p0, p1, p2, p3;
        p0 = v3(-.5f,  .5f, 0);
        p1 = v3(.5f,   .5f, 0);
        p2 = v3(.5f,  -.5f, 0);
        p3 = v3(-.5f, -.5f, 0);
        v4 Colour = v4(1.f, 0.f, 0.f, 1.f);
        Zen3DPushQuad(p0, p1, p2, p3, Colour);
    }
    {
        v3 p0, p1, p2, p3;
        p0 = v3(0.f, 1.f, 0);
        p1 = v3(1.f, 1.f, 0);
        p2 = v3(1.f, 0.f, 0);
        p3 = v3(0.f, 0.f, 0);
        v4 Colour = v4(1.f, 1.f, 0.f, 1.f);
        Zen3DPushQuad(p0, p1, p2, p3, Colour);
    }
    
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