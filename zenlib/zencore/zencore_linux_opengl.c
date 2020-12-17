// TEMP(Abi): OpenGL-y

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
global XVisualInfo * _Visual;
global GLXContext GLContext;

// IDEA(Abi): Return a linux based struct Visual->depth, Visual->visual, for use in XCreateWindow
internal void
LinuxRendererInit(void) {
    i32 WindowAttributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    _Visual = glXChooseVisual(XDisplay, 0, WindowAttributes);
    Colormap _Colourmap = XCreateColormap(XDisplay, DefaultRootWindow(XDisplay), _Visual->visual, AllocNone);
    
    _SetWindowAttributes.colormap = _Colourmap;
    
    _SetWindowAttributes.event_mask = ExposureMask | KeyPressMask;
    
    GLContext = glXCreateContext(XDisplay, _Visual, 0, GL_TRUE);
}

internal void
LinuxRendererResize(i32 Width, i32 Height) {
    glViewport(0, 0, Width, Height);
}

internal void
LinuxRendererFinalise(void) {
    glXMakeCurrent(XDisplay, XWindow, GLContext);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glXSwapBuffers(XDisplay, XWindow);
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void
LinuxSwapBuffers(void) {
    glXSwapBuffers(XDisplay, XWindow);
}

internal void *
LinuxOpenGLLoadProcedure(const char * Name) {
    void * p = glXGetProcAddressARB((const GLubyte *)Name);
    
    // WARNING(Abi): This doesn't mean that the function is supported.
    if(!p) {
        LinuxError("OpenGL Procedure Error", "Failed to load procedure");
    }
    
    return p;
}