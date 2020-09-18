
ZENAPPFUNC ZEN_APPLICATION_STATIC_LOAD(StaticLoad) {
    Platform = Platform_;
#ifdef ZENMODULE
    ZenModuleInit();
#endif
    
    AppInit();
}

ZENAPPFUNC ZEN_APPLICATION_UPDATE(Update) {
    
    AppUpdate();
    
}

#ifdef ZENMODULE
#include ZENMODULEIMPLEMENTATION
#endif