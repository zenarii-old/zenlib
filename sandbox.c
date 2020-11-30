// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

// TODO(Abi): some sort of default run data struct that can be added to
// TODO(Abi): FBOs in zen2d and at least a blur effect, plus clip.
global font Segoe;

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    Segoe = Zen2DLoadFontFromFNTAndPNG("libmono.fnt", "libmono.png");
    Zen2DSetDefaultFont(&Segoe);
}

internal void
AppUpdate() {
    Zen2DPushText("Hi there, this is a test", v2(100, 100), 16);
}


internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
    //Segoe = Zen2DLoadFontFromFNTAndPNG("mono.fnt", "mono.png");
    //Zen2DSetDefaultFont(&Segoe);
}

internal void
AppHotUnload() {
    Zen2DUnloadFont(&Segoe);
    fprintf(stderr, "[App] Hot Unloaded\n");
}