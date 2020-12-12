// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    font Segoe;
};

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
    Zen2DSetDefaultFont(&Platform->Core->Segoe);
    Platform->Core->Segoe = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
}

internal void
AppUpdate() {
    Zen2DPushText("Hi there, this is a test", v2(100, 100), 16);
}

internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}