// NOTE(Abi): Linux Headers
#include <windows.h>

// NOTE(Abi): OpenGL

// NOTE(Abi): CRT

// NOTE(Abi): Zenlib Headers
#include "zencore.h"

// NOTE(Abi): Globals
global platform GlobalPlatform;

// NOTE(Abi): Zenlib Implementations
#if 0
#ifdef USE_OPENGL
#include "zencore_linux_opengl.c"
#endif

#include "zencore_linux_time.c"
#include "zencore_linux_misc.c"
#include "zencore_linux_fileio.c"
#include "zencore_linux_app_code.c"
#endif

//~

LRESULT CALLBACK Win32ProcessEvent(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
    LRESULT Result = 0;
    switch(Message) {
        default: {
            Result = DefWindowProc(Window, Message, wParam, lParam);
        }
    }
    return Result;
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CMDLine, int ShowCommand) {
    // NOTE(abi): Create window class
    WNDCLASS WindowClass = {0};
    {
        WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        WindowClass.lpfnWndProc = Win32ProcessEvent;
        WindowClass.hInstance = Instance;
        WindowClass.lpszClassName = WINDOW_TITLE "class";
        WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
        WindowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        RegisterClass(&WindowClass);
    }
    
    
    // NOTE(abi): show the actual window.
    HWND Window = CreateWindow(WindowClass.lpszClassName, 
                               WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 
                               1280, 720,
                               0, 0, 
                               Instance, 
                               0);
    
    if(!Window) {
        DWORD Error = GetLastError();
        char Buffer[256];
        sprintf(Buffer, "ERROR: Window Creation Failed (%i)\n", Error);
        fprintf(stderr, "%s", Buffer);
        OutputDebugStringA(Buffer);
        
        goto win32_quit;
    }
    
    // TODO(Abi): Get executable directory
    
    ShowWindow(Window, ShowCommand);
    UpdateWindow(Window);
    
    // TODO(abi): Platform initialisation
    {
        GlobalPlatform.TargetFPS = 60.f;
        
        void * PermenantStorage = VirtualAlloc(0, PERMENANT_STORAGE_SIZE, 
                                               MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        GlobalPlatform.PermenantArena = MemoryArenaInit(PermenantStorage, PERMENANT_STORAGE_SIZE);
        
        void * ScratchStorage = VirtualAlloc(0, SCRATCH_STORAGE_SIZE, 
                                             MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        GlobalPlatform.ScratchArena = MemoryArenaInit(ScratchStorage, SCRATCH_STORAGE_SIZE);
        
        // NOTE(Abi): Set function pointers
        //GlobalPlatform.Error = LinuxError;
        //GlobalPlatform.LoadFile = LinuxLoadFile;
        //GlobalPlatform.HeapAlloc = LinuxHeapAlloc;
        //GlobalPlatform.HeapFree = LinuxHeapFree;
        //GlobalPlatform.GetTime = LinuxTimerGetTime;
#ifdef USE_OPENGL
        //GlobalPlatform.OpenGLLoadProcedure = LinuxOpenGLLoadProcedure;
#endif
        
        Platform = &GlobalPlatform;
    }
    
    // TODO(abi): Graphics initialisation
    
    // TODO(abi): Set up timing
    
    int AppShouldQuit = 0;
    while(!AppShouldQuit) {
        MSG message;
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            
            if(message.message == WM_QUIT) {
                AppShouldQuit = 1;
            }
        }
    }
    
    win32_quit:;
    
    return 0;
}