// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"
// TODO(Abi): sort out stride v size, sort of the memory allocations
internal void
AppUpdate() {
    //fprintf(stderr, "[App] Begin frame\n");
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