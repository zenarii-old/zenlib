/* date = September 17th 2020 10:22 pm */

#ifndef ZENCORE_PLATFORM_H
#define ZENCORE_PLATFORM_H

typedef enum key_type key_type;
enum key_type {
#define KEY(key) ZKEY_##key,
#include "zen_key_list.inc"
    ZKEY_COUNT
};

typedef enum mouse_type mouse_type;
enum mouse_type {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    
    MOUSE_BUTTON_COUNT
};

typedef struct platform platform;
struct platform {
    f64 TargetFPS;
    f32 Delta;
    
    b8 KeyDown[ZKEY_COUNT];
    b8 KeyWasDown[ZKEY_COUNT];
    b8 MouseDown[MOUSE_BUTTON_COUNT];
    b8 MouseWasDown[MOUSE_BUTTON_COUNT];
    v2 MousePosition;
    
    f32 ScreenWidth;
    f32 ScreenHeight;
    
    memory_arena PermenantArena;
    memory_arena ScratchArena;
    
    // NOTE(Abi): Function pointers
    void (*Error)(const char * Title, const char * Message);
#ifdef USE_OPENGL
    void * (*OpenGLLoadProcedure)(const char * Name);
#endif
};

global platform * Platform;

internal void
ZenPlatformBeginFrame(void) {
    MemoryCopy(Platform->KeyWasDown,   Platform->KeyDown,   sizeof(b8) * ZKEY_COUNT);
    MemoryCopy(Platform->MouseWasDown, Platform->MouseDown, sizeof(b8) * MOUSE_BUTTON_COUNT);
}

internal void
ZenPlatformEndFrame(void) {
    
}

#ifdef _MSC_VER
#define ZENAPPFUNC __declspec(dllexport)
#else
#define ZENAPPFUNC
#endif

#define ZEN_APPLICATION_STATIC_LOAD(name) void name(platform * Platform_)
typedef ZEN_APPLICATION_STATIC_LOAD(ZenApplicationStaticLoadCallback);
ZEN_APPLICATION_STATIC_LOAD(ZenApplicationStaticLoadStub) { }

#define ZEN_APPLICATION_UPDATE(name) void name(void)
typedef ZEN_APPLICATION_UPDATE(ZenApplicationUpdateCallback);
ZEN_APPLICATION_UPDATE(ZenApplicationUpdateStub) { }

#define ZEN_APPLICATION_HOT_LOAD(name) void name(platform * Platform_)
typedef ZEN_APPLICATION_HOT_LOAD(ZenApplicationHotLoadCallback);
ZEN_APPLICATION_HOT_LOAD(ZenApplicationHotLoadStub) { }

#define ZEN_APPLICATION_HOT_UNLOAD(name) void name(void)
typedef ZEN_APPLICATION_HOT_UNLOAD(ZenApplicationHotUnloadCallback);
ZEN_APPLICATION_HOT_UNLOAD(ZenApplicationHotUnloadStub) { }

#endif //ZENCORE_PLATFORM_H
