// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"
// TODO(Abi): sort out stride v size, sort of the memory allocations
internal void
AppUpdate() {
    //fprintf(stderr, "[App] Begin frame\n");
    Zen2DPushRect(v4(100, 200, 100, 100), v4(0.1, 0.2, 0.6, 1.f));
    Zen2DPushRect(v4(100, 100, 300, 200), v4(1.f, 0.3f, 0.8f, 1.f));
    Zen2DPushLine(v2(100, 100), v2(300, 200), v4(1.f, 1.f, 1.f, 1.f));
    Zen2DPushRect(v4(200 + 100, 100, 100, 100), v4(1, 0.8, 0.5, 1.f));
    
    local i32 idx = 0;
    if(Platform->KeyDown[ZKEY_A]) idx = 0;
    if(Platform->KeyDown[ZKEY_S]) idx = 1;
    if(Platform->KeyDown[ZKEY_D]) idx = 2;
    
    for(i32 j = -50; j < 50; ++j) {
        f32 Amount = Abs(j*0.02f);
        v4  Colour = v4(0.f, 0.f, 0.f, 1.f);
        switch (idx) {
            case 0: Colour.x = Amount; break;
            case 1: Colour.y = Amount; break;
            case 2: Colour.z = Amount; break;
        }
        i32 i = j * 0.4;
        Zen2DPushLine(v2(i + 50, i*i), v2(i + 51, (i+1)*(i+1)), Colour);
    }
    
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