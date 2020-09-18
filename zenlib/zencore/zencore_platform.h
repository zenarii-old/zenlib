/* date = September 17th 2020 10:22 pm */

#ifndef ZENCORE_PLATFORM_H
#define ZENCORE_PLATFORM_H

typedef enum key_type key_type;
enum key_type {
#define KEY(key) ZKEY_##key,
#include "zen_key_list.inc"
    ZKEY_COUNT
};

typedef struct platform platform;
struct platform {
    f64 TargetFPS;
    b8 KeyDown[ZKEY_COUNT];
    b8 KeyWasDown[ZKEY_COUNT];
};

global platform * Platform;

// TODO(Zen): Consider putting in zencore_platform.c? Could then use GlobalPlatform
internal void
ZenPlatformBeginFrame(void) {
    MemoryCopy(Platform->KeyWasDown, Platform->KeyDown, sizeof(b8) * ZKEY_COUNT);
}

internal void
ZenPlatformEndFrame(void) {
    
}

#endif //ZENCORE_PLATFORM_H
