// NOTE(Abi): Windows Headers and defines
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>


#define BUILD_WINDOWS

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
#include "zencore_win32_fileio.c"



//~

LRESULT CALLBACK Win32ProcessEvent(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
    LRESULT Result = 0;
    if(Message == WM_DESTROY) {
        PostQuitMessage(0);
    }
    else if((Message == WM_KEYDOWN) || (Message == WM_KEYUP)) {
        b32 IsDown = (Message == WM_KEYDOWN);
        u32 KeyCode = wParam;
        u32 KeyIndex = 0;
        
        if('A' <= KeyCode && KeyCode <= 'Z') {
            KeyIndex = KEY_A + (KeyCode - 'A');
        }
        else if(VK_LEFT <= KeyCode && KeyCode <= VK_DOWN) {
            KeyIndex = KEY_LEFT + (KeyCode - VK_LEFT);
        }
        if(KeyCode >= VK_F1 && KeyCode <= VK_F12) {
            KeyIndex = KEY_F1 + (KeyCode - VK_F1);
        }
        else if(KeyCode == VK_ESCAPE)     KeyIndex = KEY_ESCAPE;
        else if(KeyCode == VK_CONTROL)    KeyIndex = KEY_CONTROL;
        else if(KeyCode == VK_TAB)        KeyIndex = KEY_TAB;
        else if(KeyCode == VK_RETURN)     KeyIndex = KEY_RETURN;
        else if(KeyCode == VK_SHIFT)      KeyIndex = KEY_SHIFT;
        else if(KeyCode == VK_SPACE)      KeyIndex = KEY_SPACE;
        else if(KeyCode == VK_BACK)       KeyIndex = KEY_BACKSPACE;
        else if(KeyCode == VK_OEM_PERIOD) KeyIndex = KEY_PERIOD;
        
        GlobalPlatform.KeyDown[KeyIndex] = IsDown;
        if(KeyIndex)
            fprintf(stderr, "Key %d pressed\n", KeyIndex);
    }
    // TODO(Abi): WM_CHAR
    else {
        Result = DefWindowProc(Window, Message, wParam, lParam);
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
        GlobalPlatform.LoadFile = Win32LoadFile;
        //GlobalPlatform.HeapAlloc = LinuxHeapAlloc;
        //GlobalPlatform.HeapFree = LinuxHeapFree;
        //GlobalPlatform.GetTime = LinuxTimerGetTime;
#ifdef USE_OPENGL
        //GlobalPlatform.OpenGLLoadProcedure = LinuxOpenGLLoadProcedure;
#endif
        Platform = &GlobalPlatform;
    }
    
    // TODO(abi): Graphics initialisation
    
    char * f = Platform->LoadFile("text.txt", 1);
    if(f) f[254] = 0;
    Log("%s\n", f);
    
    // TODO(abi): Set up timing
    while(!GlobalPlatform.AppShouldQuit) {
        MSG message;
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            
            if(message.message == WM_QUIT) {
                GlobalPlatform.AppShouldQuit = 1;
            }
        }
    }
    
    DestroyWindow(Window);
    win32_quit:;
    
    return 0;
}