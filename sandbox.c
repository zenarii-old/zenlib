// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"
global font Arial;

internal void
AppInit() {
    fprintf(stderr, "[App] Initialised\n");
    Arial = Zen2DLoadFontFromFNTAndPNG("arial.fnt", "arial.png");
    
}

internal void
AppUpdate() {
    char * Str = "Abi Baxter, yeet \ndelete";
    Zen2DPushTextFinalN(v2(100, 100), &Arial, Str, strlen(Str));
    
}


internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}