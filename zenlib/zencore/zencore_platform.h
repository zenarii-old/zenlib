/* date = September 17th 2020 10:22 pm */

#ifndef ZENCORE_PLATFORM_H
#define ZENCORE_PLATFORM_H

typedef enum key_type key_type;
enum key_type {
#define KEY(key) KEY_##key,
#include "zen_key_list.inc"
    KEY_COUNT
};

typedef enum mouse_type mouse_type;
enum mouse_type {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    
    MOUSE_BUTTON_COUNT
};

typedef struct core core;

typedef struct platform platform;
struct platform {
    core * Core;
    b8 AppShouldQuit;
    
    f64 TargetFPS;
    f64 TotalTime;
    f32 Delta;
    
    b8 KeyDown[KEY_COUNT];
    b8 KeyWasDown[KEY_COUNT];
    b8 MouseDown[MOUSE_BUTTON_COUNT];
    b8 MouseWasDown[MOUSE_BUTTON_COUNT];
    v2 MousePosition;
    f32 MouseScroll;
    
#define MAX_PUT_CHARACTERS 32
    char PutCharacters[MAX_PUT_CHARACTERS];
    u32 PutCharactersCount;
    
    f32 ScreenWidth;
    f32 ScreenHeight;
    
    memory_arena PermenantArena;
    memory_arena ScratchArena;
    
    // NOTE(Abi): Function pointers
    void   (*Error)(const char * Title, const char * Message);
    char * (*LoadFile)(const char * Path, b32 Temporary);
    void * (*HeapAlloc)(u32 Size);
    void   (*HeapFree)(void * Memory);
    f64    (*GetTime)(void);
#ifdef USE_OPENGL
    void * (*OpenGLLoadProcedure)(const char * Name);
#endif
    
    // NOTE(Abi): Module Pointers
    void * Zen2D;
    void * Zen3D;
};

global platform * Platform;

internal void
ZenPlatformBeginFrame(void) {
    MemoryCopy(Platform->KeyWasDown,   Platform->KeyDown,   sizeof(b8) * KEY_COUNT);
    MemoryCopy(Platform->MouseWasDown, Platform->MouseDown, sizeof(b8) * MOUSE_BUTTON_COUNT);
    MemorySet(Platform->PutCharacters, 0, MAX_PUT_CHARACTERS * sizeof(char));
    Platform->PutCharactersCount = 0;
    Platform->MouseScroll = 0;
}

internal void
ZenPlatformEndFrame(void) {
    
}

internal b32
ZenMouseDown(mouse_type Button) {
    return Platform->MouseDown[Button];
}

internal b32
ZenMouseJustDown(mouse_type Button) {
    return Platform->MouseDown[Button] && !Platform->MouseWasDown[Button];
}

internal b32
ZenKeyDown(key_type Key) {
    return Platform->KeyDown[Key];
}

internal b32
ZenKeyJustDown(key_type Key) {
    return Platform->KeyDown[Key] && !Platform->KeyWasDown[Key];
}

internal void
ZenPlatformQuit(void) {
    Platform->AppShouldQuit = 1;
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
