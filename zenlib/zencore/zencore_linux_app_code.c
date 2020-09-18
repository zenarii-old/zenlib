#include <dlfcn.h>

typedef struct linux_app_code linux_app_code;
struct linux_app_code {
    ZenApplicationStaticLoadCallback * StaticLoad;
    ZenApplicationUpdateCallback * Update;
};

#define T(x) fprintf(stderr, "%s\n", x);

internal b8
LinuxAppCodeLoad(linux_app_code * AppCode) {
    b8 Success = 0;
    char * Error = 0;
    
    void * Handle;
    // TODO(Abi): maybe use RTLB_LAZY?
    // TODO(Abi): Sort out the string, should be absolute not relative
    Handle = dlopen("./sandbox.so", RTLD_LAZY);
    
    if((Error = dlerror())) {
        LinuxError("Shared Library Error", Error);
    }
    
    if(!Handle) {
        dlclose(Handle);
        return Success;
    }
    
    if((Error = dlerror())) {
        LinuxError("Shared Library Error", Error);
    }
    
    *(void**)(&AppCode->StaticLoad) = dlsym(Handle, "StaticLoad");
    *(void**)(&AppCode->Update)     = dlsym(Handle, "Update");
    
    if((Error = dlerror())) {
        LinuxError("Shared Library Error", Error);
    }
    
    // NOTE(Abi): For future: once u call dlclose you will segfault, 
    //            But will need to keep this here for the dynamic unload
    //dlclose(Handle);
    
    Success = AppCode->StaticLoad && AppCode->Update;
    
    if(!Success) {
        AppCode->StaticLoad = ZenApplicationStaticLoadStub;
        AppCode->Update     = ZenApplicationUpdateStub;
    }
    
    return Success;
}