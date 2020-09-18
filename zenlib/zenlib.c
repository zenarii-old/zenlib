// TODO(Abi): Make these the weird dll 
//https://stackoverflow.com/questions/14884126/build-so-file-from-c-file-using-gcc-command-line
//test with a simple print function
internal void ZenFirstLoad() {
    
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