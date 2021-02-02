#include <dlfcn.h>

typedef struct linux_app_code linux_app_code;
struct linux_app_code {
    ZenApplicationStaticLoadCallback * StaticLoad;
    ZenApplicationUpdateCallback     * Update;
    ZenApplicationHotLoadCallback    * HotLoad;
    ZenApplicationHotUnloadCallback  * HotUnload;
    void * Handle;
    time_t LastWriteTime;
};

internal b8
LinuxAppCodeLoad(linux_app_code * AppCode) {
    b8 Success = 0;
    char * Error = 0;
    
    void * Handle;
    // TODO(Abi): Sort out the string, should be absolute not relative
    Handle = dlopen("./sandbox.so", RTLD_NOW);
    
    if((Error = dlerror())) {
        LinuxError("Shared Library (" Stringify(__LINE__) ")", Error);
    }
    
    if(!Handle) {
        dlclose(Handle);
        return Success;
    }
    
    if((Error = dlerror())) {
        LinuxError("Shared Library (" Stringify(__LINE__) ")", Error);
    }
    
    *(void**)(&AppCode->StaticLoad) = dlsym(Handle, "StaticLoad");
    *(void**)(&AppCode->Update)     = dlsym(Handle, "Update");
    *(void**)(&AppCode->HotLoad)    = dlsym(Handle, "HotLoad");
    *(void**)(&AppCode->HotUnload)  = dlsym(Handle, "HotUnload");
    
    if((Error = dlerror())) {
        LinuxError("Shared Library (" Stringify(__LINE__) ")", Error);
    }
    
    Success = AppCode->StaticLoad && AppCode->Update && AppCode->HotLoad && AppCode->HotUnload;
    
    if(Success) {
        AppCode->Handle = Handle;
        struct stat FileAttributes = {0};
        stat("./sandbox.so", &FileAttributes);
        AppCode->LastWriteTime = FileAttributes.st_mtime;
    }
    else {
        AppCode->StaticLoad = ZenApplicationStaticLoadStub;
        AppCode->Update     = ZenApplicationUpdateStub;
        AppCode->HotLoad    = ZenApplicationHotLoadStub;
        AppCode->HotUnload  = ZenApplicationHotUnloadStub;
        dlclose(Handle);
    }
    
    return Success;
}

internal void
LinuxAppCodeBeginFrame(linux_app_code * AppCode) {
    struct stat DLLAttributes = {0};
    stat("./sandbox.so", &DLLAttributes);
    
    if(DLLAttributes.st_mtime != AppCode->LastWriteTime) {
        AppCode->HotUnload();
        
        dlclose(AppCode->Handle);
        *AppCode = (linux_app_code){0};
        LinuxTimerSleep(1000);
        LinuxAppCodeLoad(AppCode);
        
        AppCode->HotLoad(&GlobalPlatform);
    }
}