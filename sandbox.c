// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"
// TODO(Abi): sort out stride v size, sort of the memory allocations
internal void
AppUpdate() {
    //fprintf(stderr, "[App] Begin frame\n");
    Zen2DPushRect(v4(100, 200, 100, 100), v4(0.1, 0.2, 0.6, 1.f));
    Zen2DPushRect(v4(200 + 100, 100, 100, 100), v4(1, 0.8, 0.5, 1.f));
}

internal void
AppInit() {
    fprintf(stderr, "[App] Initialised\n");
}

internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}