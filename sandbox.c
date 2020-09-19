// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

internal void
AppUpdate() {
    
}

internal void
AppInit() {
    fprintf(stderr, "Begin App\n");
}

internal void
AppHotLoad() {
    fprintf(stderr, "Hot loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "Hot Unloaded\n");
}