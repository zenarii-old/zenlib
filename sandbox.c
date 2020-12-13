// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    font Font;
};

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    Zen2DSetDefaultFont(&Platform->Core->Font);
    Platform->Core->Font = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
}

internal void
AppUpdate() {
    Zen2DPushText("Hi there, this is a test", v2(100, 100), 16);
    
    Zen2DPushBlur(v4(90, 90, 90, 90));
    
    
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