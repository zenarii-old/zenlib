
typedef struct win32_appcode win32_appcode;
struct {
    ZenApplicationStaticLoadCallback * StaticLoad;
    ZenApplicationUpdateCallback     * Update;
    ZenApplicationHotLoadCallback    * HotLoad;
    ZenApplicationHotUnloadCallback  * HotUnload;
}

// !!! before moving on from this, make the dll name non fixed
internal b8
Win32AppCodeLoad(win32_app_code * AppCode) {
    
}