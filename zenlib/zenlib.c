ZENAPPFUNC ZEN_APPLICATION_STATIC_LOAD(StaticLoad) {
    Platform = Platform_;
    
#ifdef ZEN2D
    Zen2D = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(Zen2D));
    Zen2DInit(&Platform->PermenantArena);
#endif
    
    AppInit();
    
}

ZENAPPFUNC ZEN_APPLICATION_UPDATE(Update) {
#ifdef ZEN2D
    Zen2DBeginFrame();
#endif
    AppUpdate();
    
#ifdef ZEN2D
    Zen2DEndFrame();
#endif
}

ZENAPPFUNC ZEN_APPLICATION_HOT_LOAD(HotLoad) {
    Platform = Platform_;
    
    // TODO(Abi): Lose all the pointers so need to get them back
    
    AppHotLoad();
}

ZENAPPFUNC ZEN_APPLICATION_HOT_UNLOAD(HotUnload) {
    AppHotUnload();
}

#ifdef ZEN2D
#include "zen2d/zen2d.c"
#endif