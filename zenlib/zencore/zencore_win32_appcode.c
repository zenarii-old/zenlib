
typedef struct win32_app_code win32_app_code;
struct win32_app_code {
    HMODULE Handle;
    FILETIME LastWriteTime;
    ZenApplicationStaticLoadCallback * StaticLoad;
    ZenApplicationUpdateCallback     * Update;
    ZenApplicationHotLoadCallback    * HotLoad;
    ZenApplicationHotUnloadCallback  * HotUnload;
};

internal void
Win32AppCodeUnload(win32_app_code * AppCode) {
    if(AppCode->Handle) {
        FreeLibrary(AppCode->Handle);
        AppCode->Handle = 0;
    }
    
    AppCode->StaticLoad = ZenApplicationStaticLoadStub;
    AppCode->Update     = ZenApplicationUpdateStub;
    AppCode->HotLoad    = ZenApplicationHotLoadStub;
    AppCode->HotUnload  = ZenApplicationHotUnloadStub;
}

// TODO(Abi) before moving on from this, make the dll name non fixed/ non relative?
internal b8
Win32AppCodeLoad(win32_app_code * AppCode) {
    b8 Success = 0;
    
    char * DLLPath    = "./" DLL_NAME ".dll";
    char * HandlePath = "./" DLL_NAME "_temp.dll";
    CopyFile(DLLPath, HandlePath, FALSE);
    
    HMODULE Handle = LoadLibraryA(HandlePath);
    if(!Handle) {
        LogError("Failed to load dll at %s", HandlePath);
        return Success;
    }
    
    AppCode->Handle = Handle;
    *(void**)(&AppCode->StaticLoad) = GetProcAddress(Handle, "StaticLoad");
    *(void**)(&AppCode->Update)     = GetProcAddress(Handle, "Update");
    *(void**)(&AppCode->HotLoad)    = GetProcAddress(Handle, "HotLoad");
    *(void**)(&AppCode->HotUnload)  = GetProcAddress(Handle, "HotUnload");
    
    Success = AppCode->StaticLoad && AppCode->Update && AppCode->HotLoad && AppCode->HotUnload;
    
    if(Success) {
        AppCode->LastWriteTime = Win32GetLastWriteTime(DLLPath);
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
    char * DLLPath = "./" DLL_NAME ".dll";
    FILETIME NewDLLWriteTime = Win32GetLastWriteTime(DLLPath);
    
    if(CompareFileTime(&NewDLLWriteTime, &AppCode->LastWriteTime)) {
        AppCode->HotUnload();
        Win32AppCodeUnload(AppCode);
        // NOTE(abiab): Sleep appears to be required for it to properly reload
        Sleep(200);
        Win32AppCodeLoad(AppCode);
        AppCode->HotLoad(&GlobalPlatform);
    }
}