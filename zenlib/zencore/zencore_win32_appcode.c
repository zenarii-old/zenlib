
typedef struct win32_app_code win32_app_code;
struct win32_app_code {
    ZenApplicationStaticLoadCallback * StaticLoad;
    ZenApplicationUpdateCallback     * Update;
    ZenApplicationHotLoadCallback    * HotLoad;
    ZenApplicationHotUnloadCallback  * HotUnload;
};

// !!! before moving on from this, make the dll name non fixed
internal b8
Win32AppCodeLoad(win32_app_code * AppCode) {
    b8 Success = 0;
    
    char * HandlePath = "./" DLL_NAME ".dll";
    HMODULE Handle = LoadLibraryA(HandlePath);
    if(!Handle) {
        LogError("Failed to load dll at %s", HandlePath);
        return Success;
    }
    
    *(void**)(&AppCode->StaticLoad) = GetProcAddress(Handle, "StaticLoad");
    *(void**)(&AppCode->Update)     = GetProcAddress(Handle, "Update");
    *(void**)(&AppCode->HotLoad)    = GetProcAddress(Handle, "HotLoad");
    *(void**)(&AppCode->HotUnload)  = GetProcAddress(Handle, "HotUnload");
    
    Success = AppCode->StaticLoad && AppCode->Update && AppCode->HotLoad && AppCode->HotUnload;
    
    if(Success) {
        // TODO(abi): hot loading
    }
    else {
        AppCode->StaticLoad = ZenApplicationStaticLoadStub;
        AppCode->Update     = ZenApplicationUpdateStub;
        AppCode->HotLoad    = ZenApplicationHotLoadStub;
        AppCode->HotUnload  = ZenApplicationHotUnloadStub;
    }
    
    return Success;
}

internal void
Win32AppCodeBeginFrame(win32_app_code * AppCode) {
    
}