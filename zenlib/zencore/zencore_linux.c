// NOTE(Abi): Linux Headers
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#undef Success

// NOTE(Abi): OpenGL
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

// NOTE(Abi): CRT

// NOTE(Abi): Zenlib Headers
#include "zencore.h"
#include "zencore_linux_time.h"
#include "zencore_linux_misc.h"

// NOTE(Abi): Globals
global Display * XDisplay;
global Window XWindow;
global XSetWindowAttributes _SetWindowAttributes;
global Atom WindowCloseID;
global platform GlobalPlatform;

// NOTE(Abi): Zenlib Implementations
#ifdef USE_OPENGL
#include "zencore_linux_opengl.c"
#endif

#include "zencore_linux_time.c"
#include "zencore_linux_misc.c"
#include "zencore_linux_fileio.c"
#include "zencore_linux_app_code.c"


//~


internal void
LinuxProcessEvent(XEvent Event) {
    switch(Event.type) {
        case Expose: {
            XWindowAttributes attribs;
            XGetWindowAttributes(XDisplay, XWindow, &attribs);
            LinuxRendererResize(attribs.width, attribs.height);
            GlobalPlatform.ScreenWidth  = attribs.width;
            GlobalPlatform.ScreenHeight = attribs.height;
        } break;
        
        case KeyPress:
        case KeyRelease: {
            b32 KeyDown = (Event.type == KeyPress);
            int KeyIndex = 0;
            KeySym KeySymbol = XkbKeycodeToKeysym(XDisplay, Event.xkey.keycode, 0, Event.xkey.state & ShiftMask ? 1 : 0);
            char PutChar = 0;
            unused(PutChar);
            //TODO(Zen): Fill out with symbols and spaces etc.
            switch(KeySymbol) {
                case XK_Escape: {
                    KeyIndex = ZKEY_ESCAPE;
                } break;
                case XK_Return: {
                    KeyIndex = ZKEY_RETURN;
                    PutChar = '\n';
                } break;
                case XK_space: {
                    KeyIndex = ZKEY_SPACE;
                    PutChar = ' ';
                } break;
                case XK_BackSpace: {
                    KeyIndex = ZKEY_BACKSPACE;
                    PutChar = '\b';
                } break;
                case XK_period: {
                    KeyIndex = ZKEY_PERIOD;
                    PutChar = '.';
                } break;
                case XK_comma: {
                    KeyIndex = ZKEY_COMMA;
                    PutChar = ',';
                } break;
                
                default: {
                    if(KeySymbol >= XK_A && KeySymbol <= XK_Z) {
                        KeyIndex = ZKEY_A + KeySymbol - XK_A;
                        PutChar = 'A' + KeySymbol - XK_A;
                    }
                    else if(KeySymbol >= XK_a && KeySymbol <= XK_z) {
                        KeyIndex = ZKEY_A + KeySymbol - XK_a;
                        PutChar = 'a' + KeySymbol - XK_a;
                    }
                    else if(KeySymbol >= XK_F1 && KeySymbol <= XK_F12) {
                        KeyIndex = ZKEY_F1 + KeySymbol - XK_F1;
                    }
                    else {
                        ZSoftAssert(!"Unknown character");
                    }
                } break;
            }
            GlobalPlatform.KeyDown[KeyIndex] = KeyDown;
            if(KeyDown && PutChar && GlobalPlatform.PutCharactersCount < MAX_PUT_CHARACTERS) {
                GlobalPlatform.PutCharacters[GlobalPlatform.PutCharactersCount++]
                    = PutChar;
            }
        } break;
        
        // NOTE(Abi): Mouse Buttons
        case ButtonPress:
        case ButtonRelease: {
            b32 ButtonDown = (Event.type == ButtonPress);
            int ButtonIndex = 0;
            switch (Event.xbutton.button) {
                case Button1: {
                    ButtonIndex = MOUSE_BUTTON_LEFT;
                } break;
                case Button2: {
                    ButtonIndex = MOUSE_BUTTON_MIDDLE;
                } break;
                case Button3: {
                    ButtonIndex = MOUSE_BUTTON_RIGHT;
                } break;
                case Button4: {
                    if(ButtonDown) Platform->MouseScroll += 1.f/12.f;
                } break;
                case Button5: {
                    if(ButtonDown) Platform->MouseScroll -= 1.f/12.f;
                } break;
            }
            
            if(ButtonIndex < MOUSE_BUTTON_COUNT)
                GlobalPlatform.MouseDown[ButtonIndex] = ButtonDown;
            
        } break;
        
        case MotionNotify: {
            GlobalPlatform.MousePosition = v2(Event.xmotion.x, GlobalPlatform.ScreenHeight - Event.xmotion.y);
        } break;
        
    }
}

int main(int argc, char ** argv) {
    XDisplay = XOpenDisplay(0);
    
    b32 LoadTimerSuccess = LinuxTimerInit();
    if(!LoadTimerSuccess) {
        LinuxError("Fatal error", "Failed to load timer.");
        goto ZenLinuxEnd;
    }
    
    
    linux_app_code LinuxAppCode = {0};
    b32 LoadAppCodeSuccess = LinuxAppCodeLoad(&LinuxAppCode);
    if(!LoadAppCodeSuccess) {
        LinuxError("Fatal error", "Failled to load app code.");
        goto ZenLinuxEnd;
    }
    
    
    //NOTE(Abi): Create Window
    {
        LinuxRendererInit();
        
        XWindow = XCreateWindow(XDisplay, DefaultRootWindow(XDisplay), 
                                0, 0, WINDOW_SIZE, 
                                0, _Visual->depth, InputOutput, _Visual->visual, CWColormap | CWEventMask,
                                &_SetWindowAttributes);
        LinuxRendererFinalise();
        
        // NOTE(Abi): Force an expose event to set ScreenWidth/Height
        {
            XEvent ExposeEvent = {0};
            ExposeEvent.type   = Expose;
            LinuxProcessEvent(ExposeEvent);
            Assert((GlobalPlatform.ScreenWidth > 0) && (GlobalPlatform.ScreenHeight > 0));
        }
        
        XStoreName(XDisplay, XWindow, WINDOW_TITLE);
        
        XSelectInput(XDisplay, XWindow,
                     SubstructureNotifyMask
                     | ExposureMask
                     | PointerMotionMask
                     | ButtonPressMask
                     | ButtonReleaseMask
                     | KeyPressMask
                     | KeyReleaseMask);
        
        XMapRaised(XDisplay, XWindow);
        
        WindowCloseID = XInternAtom(XDisplay, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(XDisplay, XWindow, &WindowCloseID, 1);
        
        if(!XWindow) {
            LinuxError("Fatal Error", "Failed to create window");
            goto ZenLinuxEnd;
        }
    }
    
    // NOTE(Abi): Initialise platform
    {
        GlobalPlatform.TargetFPS = 60.f;
        
        void * PermenantStorage = calloc(PERMENANT_STORAGE_SIZE, 1);
        GlobalPlatform.PermenantArena = MemoryArenaInit(PermenantStorage, PERMENANT_STORAGE_SIZE);
        
        void * ScratchStorage = calloc(SCRATCH_STORAGE_SIZE, 1);
        GlobalPlatform.ScratchArena = MemoryArenaInit(ScratchStorage, SCRATCH_STORAGE_SIZE);
        
        // NOTE(Abi): Set function pointers
        GlobalPlatform.Error = LinuxError;
        GlobalPlatform.LoadFile = LinuxLoadFile;
        GlobalPlatform.HeapAlloc = LinuxHeapAlloc;
        GlobalPlatform.HeapFree = LinuxHeapFree;
        GlobalPlatform.GetTime = LinuxTimerGetTime;
#ifdef USE_OPENGL
        GlobalPlatform.OpenGLLoadProcedure = LinuxOpenGLLoadProcedure;
#endif
        
        Platform = &GlobalPlatform;
    }
    
    LinuxAppCode.StaticLoad(&GlobalPlatform);
    LinuxAppCode.HotLoad(&GlobalPlatform);
    
    while (!Platform->AppShouldQuit) {
        ZenPlatformBeginFrame();
        LinuxTimerBeginFrame();
        LinuxAppCodeBeginFrame(&LinuxAppCode);
        
        // NOTE(Abi): Process Input
        while (XPending(XDisplay) > 0) {
            XEvent Event;
            XNextEvent(XDisplay, &Event);
            
            if (Event.type == ClientMessage && Event.xclient.data.l[0] == WindowCloseID) {
                Platform->AppShouldQuit = 1;
            }
            else {
                LinuxProcessEvent(Event);
            }
        }
        
        MemoryArenaClear(&GlobalPlatform.ScratchArena);
        LinuxAppCode.Update();
        
        LinuxSwapBuffers();
        LinuxTimerEndFrame();
    }
    
    ZenLinuxEnd:;
    
    return 0;
}
