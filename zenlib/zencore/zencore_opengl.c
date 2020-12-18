#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#define OPENGLPROC(function, type) global PFNGL##type##PROC gl##function;
#include "opengl_proc_list.inc"

internal void
OpenGLLoadAllFunctions(void) {
#define OPENGLPROC(function, type) gl##function = (PFNGL##type##PROC)Platform->OpenGLLoadProcedure("gl" #function);
#include "opengl_proc_list.inc"
}

//
// ~Framebuffer
//

typedef struct framebuffer framebuffer;
struct framebuffer {
    GLuint ID;
    GLuint Texture;
    GLuint Depth;
    GLint Width, Height;
};

internal framebuffer
OpenGLCreateFramebuffer(GLint Width, GLint Height) {
    GLuint ID = 0;
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    
    GLuint Texture = 0;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);
    
    GLuint Depth = 0;
    glGenTextures(1, &Depth);
    glBindTexture(GL_TEXTURE_2D, Depth);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, Depth, 0);
    }
    
    Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    framebuffer Framebuffer = {0};
    Framebuffer.ID = ID;
    Framebuffer.Texture = Texture;
    Framebuffer.Depth = Depth;
    Framebuffer.Width = Width;
    Framebuffer.Height = Height;
    
    return Framebuffer;
}

internal void
OpenGLBindFramebuffer(framebuffer * Framebuffer) {
    if(Framebuffer) glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer->ID);
    else            glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

internal void
Zen2DDeleteFramebuffer(framebuffer * Framebuffer) {
    glDeleteFramebuffers(1, &Framebuffer->ID);
    glDeleteTextures(1, &Framebuffer->Texture);
    glDeleteTextures(1, &Framebuffer->Depth);
    
    *Framebuffer = (framebuffer){0};
}