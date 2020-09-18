// NOTE(Abi): Linux Headers
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#undef Success
// NOTE(Abi): OpenGL

// NOTE(Abi): CRT

// NOTE(Abi): Zenlib Headers
#include "zencore.h"
#include "zencore_linux_time.h"

// NOTE(Abi): Globals
global Display * XDisplay;
global Window XWindow;
global XSetWindowAttributes _SetWindowAttributes;
global Colormap _Colourmap;
global Atom WindowCloseID;
global platform GlobalPlatform;

// NOTE(Abi): Zenlib Implementations
#include "zencore_linux_time.c"
#include "zencore_linux_app_code.c"


// TEMP(Abi): OpenGL-y

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
global XVisualInfo * _Visual;

// IDEA(Abi): Return a linux based struct Visual->depth, Visual->visual, for use in XCreateWindow
internal void
TEMPRendererInit() {
    i32 WindowAttributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    _Visual = glXChooseVisual(XDisplay, 0, WindowAttributes);
    _Colourmap = XCreateColormap(XDisplay, DefaultRootWindow(XDisplay), _Visual->visual, AllocNone);
    
    _SetWindowAttributes.colormap = _Colourmap;
    //TODO(Zen): Check if need more, since in ZCreateWindow using more in the XSelectInput
    _SetWindowAttributes.event_mask = ExposureMask | KeyPressMask;
}

//~


internal void
LinuxProcessEvent(XEvent Event) {
    switch(Event.type) {
        /* TODO
        case ConfigureNotify: {
            ZRendererResize(Event.xconfigurerequest.width, Event.xconfigurerequest.height);
        } break;
*/
        case Expose: {
            XWindowAttributes attribs;
            XGetWindowAttributes(XDisplay, XWindow, &attribs);
            //TODO ZRendererResize(attribs.width, attribs.height);
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
                default: {
                    if(KeySymbol >= XK_A && KeySymbol <= XK_Z) {
                        KeyIndex = ZKEY_A + KeySymbol - XK_A;
                        PutChar = 'A' + KeySymbol - XK_A;
                    }
                    else if(KeySymbol >= XK_a && KeySymbol <= XK_z) {
                        KeyIndex = ZKEY_A + KeySymbol - XK_a;
                        PutChar = 'a' + KeySymbol - XK_a;
                    }
                    else {
                        ZSoftAssert(!"Unknown character");
                    }
                } break;
            }
            GlobalPlatform.KeyDown[KeyIndex] = KeyDown;
            //TODO if(KeyDown && PutChar) _ZPutCharacter(PutChar);
            if(KeyDown && PutChar) fprintf(stderr, "%c", PutChar);
        } break;
        /*TODO
        //NOTE(Zen): Mouse Buttons
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
            }
            GlobalPlatform.MouseDown[ButtonIndex] = ButtonDown;
        } break;
*/
        /* TODO
        case MotionNotify: {
            GlobalPlatform.MousePosition = v2(Event.xmotion.x, GlobalPlatform.ScreenHeight - Event.xmotion.y);
        } break;
*/
    }
}

#define TEMP_WINDOW_DIMENSIONS 800, 600
#define TEMP_WINDOW_NAME "zenlib"
int main(int argc, char ** argv) {
    XDisplay = XOpenDisplay(0);
    
    b32 LoadTimerSuccess = LinuxTimerInit();
    if(!LoadTimerSuccess) {
        //TODO(Zen): Debug-out
        fprintf(stderr, "Failed to initialise timer");
    }
    
    TEMPRendererInit();
    //LinuxRendererInit();
    
    //NOTE(Abi): Create Window
    {
        XWindow = XCreateWindow(XDisplay, DefaultRootWindow(XDisplay), 
                                0, 0, TEMP_WINDOW_DIMENSIONS, 
                                0, _Visual->depth, InputOutput, _Visual->visual, CWColormap | CWEventMask,
                                &_SetWindowAttributes);
        //LinuxRendererFinalise();
        
        XStoreName(XDisplay, XWindow, TEMP_WINDOW_NAME);
        
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
    } // TODO(Abi): I guess check if this fails lol
    
    // NOTE(Abi): Initialise platform
    {
        GlobalPlatform.TargetFPS = 60.f;
        Platform = &GlobalPlatform;
    }
    
    //LinuxAppCode.FirstLoad();
    //LinuxAppCode.HotLoad();
    
    // TODO(Abi): Platform struct
    b8 AppShouldQuit = 0;
    while (!AppShouldQuit) {
        ZenPlatformBeginFrame();
        LinuxTimerBeginFrame();
        
        // NOTE(Abi): Process Input
        while (XPending(XDisplay) > 0) {
            XEvent Event;
            XNextEvent(XDisplay, &Event);
            
            if (Event.type == ClientMessage && Event.xclient.data.l[0] == WindowCloseID) {
                AppShouldQuit = 1;
            }
            else {
                LinuxProcessEvent(Event);
            }
        }
        
        // TODO(Abi): Watch the handmade hero episode, call AppCode->Update();
        //LinuxAppCode.Update();
        
        //TODO(Zen): Timing
        
        LinuxTimerEndFrame();
    }
    
    return 0;
}