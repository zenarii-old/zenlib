// NOTE(Abi): Linux Headers
#include <X11/Xlib.h>

// NOTE(Abi): OpenGL

// NOTE(Abi): CRT

// NOTE(Abi): Zenlib Headers
#include "zencore.h"
#if 0
#include "zencore_linux_timer.h"
#endif

// NOTE(Abi): Globals
global Display * XDisplay;
global Window XWindow;
global XSetWindowAttributes _SetWindowAttributes;
global Colormap _Colourmap;
global Atom WindowCloseID;

// NOTE(Abi): Zenlib Implementations // TODO(Abi): sjdjasklsdja
#if 0
#include "znecore_linux_timer.c"
#endif

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

#define TEMP_WINDOW_DIMENSIONS 800, 600
#define TEMP_WINDOW_NAME "zenlib"
int main() {
    XDisplay = XOpenDisplay(0);
    
    TEMPRendererInit();
    //LinuxRendererInit();
    
    //NOTE(Abi): Create Window
    {
        XWindow = XCreateWindow(XDisplay, DefaultRootWindow(XDisplay), 
                                0, 0, 800, 600, 
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
    
    // TODO(Abi): Platform struct
    b8 AppShouldQuit = 0;
    while (!AppShouldQuit) {
        // NOTE(Abi): Process Input
        while (XPending(XDisplay) > 0) {
            XEvent Event;
            XNextEvent(XDisplay, &Event);
            
            if (Event.type == ClientMessage && Event.xclient.data.l[0] == WindowCloseID) {
                AppShouldQuit = 1;
            }
            else {
                //LinuxProcessEvent(Event);
            }
        }
    }
    
    return 0;
}