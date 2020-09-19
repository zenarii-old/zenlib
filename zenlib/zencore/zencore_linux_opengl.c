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
    //TODO(Zen): Check if need more, since in ZCreateWindow using more in the XSelectInput
    _SetWindowAttributes.event_mask = ExposureMask | KeyPressMask;
    
    GLContext = glXCreateContext(XDisplay, _Visual, 0, GL_TRUE);
}

internal void
LinuxRendererFinalise(void) {
    glXMakeCurrent(XDisplay, XWindow, GLContext);
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
        LinuxError("OpenGL Procedure Error", "Failed to load procedure: %s", Name);
    }
    
    return p;
}