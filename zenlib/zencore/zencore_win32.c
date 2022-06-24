

// NOTE(Abi): Windows Headers and defines
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define BUILD_WINDOWS

// NOTE(Abi): OpenGL
#include <gl/gl.h>
#include "ext/glext.h"
#include "ext/wglext.h"

// NOTE(Abi): CRT

// NOTE(Abi): Zenlib Headers
#include "zencore.h"

// NOTE(abiab): FreeType

// NOTE(abiab): necessary as "internal" is used a few times in the free type library
//              so having internal defined causes errors
#ifdef internal
#undef internal

#include "ft2build.h"
#include FT_FREETYPE_H

#define internal static
#endif


// NOTE(Abi): Globals
global platform GlobalPlatform;
global FT_Library GlobalFreeType;

// NOTE(Abi): Zenlib Implementations
#ifdef USE_OPENGL
#include "zencore_win32_opengl.c"
#endif
#include "zencore_win32_fileio.c"
#include "zencore_win32_timer.c"
#include "zencore_win32_appcode.c"
#include "zencore_win32_misc.c"
//
// ~Fonting
//
// TODO(abiab): move this to its own file for both win/linux
// TODO(abiab): make not so opengl specific

// probs need a 2D array to fill the texture then fill each specifically alongside the character data.
// TODO(abiab): Change this to font glyph etc once replaced the other system

internal void *
Win32LoadFont(const char * Path, i32 FontHeight, void ** Glyphs) {
    i32 DataLength = 0;
    char * Data = Platform->LoadFile(Path, 0, &DataLength);    
    
    FT_Face Face = {0};
    i32 Error = FT_New_Memory_Face(GlobalFreeType, Data, DataLength, 0, &Face);
    
    if(Error == FT_Err_Unknown_File_Format) {
        LogWarning("Failed to load font %s, unrecognised file format", Path);
    }
    else if(Error) {
        LogWarning("Failed to load font %s", Path);
    }
    
    Error = FT_Set_Pixel_Sizes(Face, 0, FontHeight);
    if (Error) LogWarning("Attempted to set font %s pizel size to an unlisted value", Path);
    
    // NOTE(Abi): Assuming that the max dimensions of each glyph are FontHeight x FontHeight
    // NOTE(Abi): bitmap will be 10 glyphs by 10 glyphs as min(Square) > 95 is 100.
    // TODO(abiab): check font width
    i32 FontBitmapSize = (10 * FontHeight) * (10 * FontHeight) * sizeof(u8);
    u8 * FontBitmap = MemoryArenaAlloc(&Platform->ScratchArena, FontBitmapSize);
    
    character * Characters = Platform->HeapAlloc(sizeof(character) * 95);
    character * CurrentCharacter = 0;
    *Glyphs = Characters;
    
    for(i32 CharacterCode = ' '; CharacterCode < 127; ++CharacterCode) {
        Error = FT_Load_Char(Face, CharacterCode, FT_LOAD_RENDER);
        if(Error) LogWarning("Failed to load character %c in font %s", CharacterCode, Path);
        
        // Note(Abi): Fill in the glyph info
        CurrentCharacter = &Characters[CharacterCode - ' '];
        {
            CurrentCharacter->XOffset  = Face->glyph->bitmap_left;
            CurrentCharacter->YOffset  = Face->glyph->bitmap_top;
            CurrentCharacter->XAdvance = Face->glyph->advance.x / 64;
            CurrentCharacter->Width    = Face->glyph->bitmap.width;
            CurrentCharacter->Height   = Face->glyph->bitmap.rows;
            CurrentCharacter->x        = ((CharacterCode - ' ') % 10) * FontHeight;
            CurrentCharacter->y        = ((CharacterCode - ' ') / 10) * FontHeight;
        }
        
        // NOTE(Abi): Draw to bitmap
        i32 XGlyph = (CharacterCode - ' ') % 10;
        i32 YGlyph = (CharacterCode - ' ') / 10;
        
        i32 XCursor = XGlyph * FontHeight;
        i32 YCursor = YGlyph * FontHeight;
        
        for(i32 x = 0; x < CurrentCharacter->Width; ++x) {
            for(i32 y = 0; y < CurrentCharacter->Height; ++y) {
                i32 XWrite = XCursor + x;
                i32 YWrite = YCursor + y;
                FontBitmap[XWrite + YWrite * (10 * FontHeight)] =
                ((u8*)Face->glyph->bitmap.buffer)[x + y * Face->glyph->bitmap.width];
            }
        }
        
        // move the position of the drawer
    }
    
    FT_Done_Face(Face);
    
    return FontBitmap;
}


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
    else if(Message == WM_SIZE) {
        RECT ClientRect = {0};
        GetClientRect(Window, &ClientRect);
        i32 Width = ClientRect.right - ClientRect.left;
        i32 Height = ClientRect.bottom - ClientRect.top;
        
        Win32RendererResize(Width, Height);
        GlobalPlatform.ScreenWidth  = Width;
        GlobalPlatform.ScreenHeight = Height;
        Log("resize, %d, %d", Width, Height);
    }
    else if (Message == WM_LBUTTONDOWN || Message == WM_LBUTTONUP) {
        b32 IsDown = (Message == WM_LBUTTONDOWN);
        GlobalPlatform.MouseDown[0] = IsDown;
    }
    else if (Message == WM_RBUTTONDOWN || Message == WM_RBUTTONUP) {
        b32 IsDown = (Message == WM_RBUTTONDOWN);
        GlobalPlatform.MouseDown[2] = IsDown;
    }
    else if(Message == WM_MOUSEWHEEL) {
        i32 MouseDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        //Note(Zen): Get Mouse Scroll between 0 and 1
        GlobalPlatform.MouseScroll = MouseDelta / 120.f;
    }
    else if(Message == WM_SIZING) {
        
    }
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
                               WINDOW_SIZEX, WINDOW_SIZEY,
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
    
    
    win32_app_code Win32AppCode = {0};
    b32 LoadAppCodeSuccess = Win32AppCodeLoad(&Win32AppCode);
    if(!LoadAppCodeSuccess) {
        LogError("Fatal Error: Failed to load appcode");
        goto win32_quit;
    }
    
    // NOTE(abiab): load freetype library
    if (FT_Init_FreeType(&GlobalFreeType)) {
        LogError("Fatal Error: Failed to initialise FreeType Library");
        goto win32_quit;
    }
    
    // TODO(Abi): Get executable directory
    
    ShowWindow(Window, ShowCommand);
    UpdateWindow(Window);
    
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
        GlobalPlatform.HeapAlloc = Win32HeapAlloc;
        GlobalPlatform.HeapFree = Win32HeapFree;
        GlobalPlatform.GetTime = Win32TimerGetTime;
#ifdef USE_OPENGL
        GlobalPlatform.OpenGLLoadProcedure = Win32OpenGLLoadFunction;
#endif
        GlobalPlatform.LoadFontData = Win32LoadFont;
        
        Platform = &GlobalPlatform;
    }
    
    
    // NOTE(abi): Graphics Initialisation
    HDC DeviceContext = GetDC(Window);
    Win32RendererInit(DeviceContext);
    
    // TODO(abi): move to match linux platform layer
    Win32TimerInit();
    
    Win32AppCode.StaticLoad(&GlobalPlatform);
    Win32AppCode.HotLoad(&GlobalPlatform);
    
    while(!GlobalPlatform.AppShouldQuit) {
        ZenPlatformBeginFrame();
        //Win32AppCodeBeginFrame();
        
        // NOTE(abi): Process input
        MSG message;
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            
            if(message.message == WM_QUIT) {
                GlobalPlatform.AppShouldQuit = 1;
            }
        }
        SwapBuffers(DeviceContext);
        
        MemoryArenaClear(&GlobalPlatform.ScratchArena);
        Win32AppCode.Update();
        
        Win32TimerEndFrame();
    }
    
    DestroyWindow(Window);
    win32_quit:;
    
    return 0;
}