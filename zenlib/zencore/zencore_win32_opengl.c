#include <gl/gl.h>
#include "ext/glext.h"
#include "ext/wglext.h"

global HGLRC GlobalOpenGLContext;
global PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
global PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
global PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;


internal void *
Win32OpenGLLoadFunction(const char* Name) {
    void * p = (void *)wglGetProcAddress(Name);
    if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        //attempt to load OpenGL 1.1 proc
        HMODULE Module = LoadLibraryA("opengl32.dll");
        p = GetProcAddress(Module, Name);
    }
    return p;
}

internal b32
Win32RendererInit(HDC DeviceContext) {
    b32 Success = 0;
    PIXELFORMATDESCRIPTOR DummyPixelFormatDesc = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, //version
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
        PFD_TYPE_RGBA,
        32, //Color depth
        0, 0, 0, 0, 0, 0,
	    0,
	    0,
	    0,
	    0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int PixelFormat = ChoosePixelFormat(DeviceContext, &DummyPixelFormatDesc);
    SetPixelFormat(DeviceContext, PixelFormat, &DummyPixelFormatDesc);
    
    HGLRC DummyOpenGLContext = wglCreateContext(DeviceContext);
    wglMakeCurrent(DeviceContext, DummyOpenGLContext);
    
    //Note(Zen): load the opengl functions
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)Win32OpenGLLoadFunction("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)Win32OpenGLLoadFunction("wglCreateContextAttribsARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)Win32OpenGLLoadFunction("wglSwapIntervalEXT");
    
    //Note(Zen): create the real pixel format
    {
        //Note(Zen): Attributes taken from: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Pixel_Format_Extensions
        int PixelFormatAttributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            //WGL_SAMPLE_BUFFERS_ARB, 1,
            //WGL_SAMPLES_ARB, 4,  
            0
        };
        UINT AttributeCount = 0;
        wglChoosePixelFormatARB(DeviceContext, PixelFormatAttributes, 0, 1, &PixelFormat, &AttributeCount);
    }
    
    {
        int ContextAttributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            0
        };
        
        GlobalOpenGLContext = wglCreateContextAttribsARB(DeviceContext, DummyOpenGLContext, ContextAttributes);
        
        if(GlobalOpenGLContext) {
            wglMakeCurrent(DeviceContext, 0);
            wglDeleteContext(DummyOpenGLContext);
            wglMakeCurrent(DeviceContext, GlobalOpenGLContext);
            wglSwapIntervalEXT(0);
            Success = 1;
        }
        
    }
    return Success;
}

internal void
Win32RendererResize(i32 Width, i32 Height) {
    glViewport(0, 0, Width, Height);
}
