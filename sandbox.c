// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"


internal void
AppInit() {
    fprintf(stderr, "[App] Loaded");
}

internal void
AppUpdate() {
    Zen2DPushRect(v4(100, 100, 100, 100), v4(1.f, 1.f, 1.f, 1.f));
}


internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
    Log("func: %p", glUseProgram);
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}