
internal void ZenLoad() {
    
}

internal void ZenUpdate() {
#ifdef ZENMODULE
    ZenModuleBeginFrame();
#endif
    
    AppUpdate();
    
#ifdef ZENMODULE
    ZenModuleEndFrame();
#endif
}

#ifdef ZENMODULE
#include ZENMODULEIMPLEMENTATION
#endif