// TODO(abi): figure out like, where i had put this for the linux build
#include <windows.h>
#include <gl/gl.h>
#include "ext/glext.h"
#include "ext/wglext.h"

typedef GLuint shader;

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
OpenGLDeleteFramebuffer(framebuffer * Framebuffer) {
    glDeleteFramebuffers(1, &Framebuffer->ID);
    glDeleteTextures(1, &Framebuffer->Texture);
    glDeleteTextures(1, &Framebuffer->Depth);
    
    *Framebuffer = (framebuffer){0};
}

//
//
//

internal void
OpenGLAddFloatAttribute(i32 ID, u32 Count, u32 Stride, u32 Offset) {
    glVertexAttribPointer(ID, Count, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void *)(Offset*sizeof(f32)));
    glEnableVertexAttribArray(ID);
}

// TODO NOTE TODO
// ~Shaders
// TODO NOTE TODO

//
// ~Textures
//
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ASSERT(x) Assert(x)
// TODO(Abi): Add scratch arena stuff so no allocations
#include "ext/stb_image.h"

#define ZEN_TEXTURE_NEAREST (1<<0)
#define ZEN_TEXTURE_LINEAR  (1<<1)

typedef struct texture texture;
struct texture {
    GLuint ID;
    f32 Width;
    f32 Height;
};

internal texture
ZenLoadTexture(unsigned char * Data, i32 Width, i32 Height, i32 Channels, u32 Flags) {
    texture Texture = {0};
    {
        glGenTextures(1, &Texture.ID);
        Texture.Width  = Width;
        Texture.Height = Height;
    }
    glBindTexture(GL_TEXTURE_2D, Texture.ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    {
        u32 Magnification = GL_NEAREST;
        if(Flags & ZEN_TEXTURE_LINEAR) Magnification = GL_LINEAR;
        else if(Flags & ZEN_TEXTURE_NEAREST) Magnification = GL_NEAREST;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Magnification);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Magnification);
    }
    
    switch (Channels) {
        case 4: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        } break;
        case 3: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        } break;
        case 2: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, Width, Height, 0, GL_RG, GL_UNSIGNED_BYTE, Data);
        } break;
        case 1: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, Data);
        } break;
        
        default: Assert(!"Invalid number of channels");
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return Texture;
}


internal texture
ZenLoadTextureFromPNG(const char * Path, u32 Flags) {
    i32 Width, Height, Channels;
    unsigned char * Data = stbi_load(Path, &Width, &Height, &Channels, 0);
    
    texture T;
    if(Data) {
        T = ZenLoadTexture(Data, Width, Height, Channels, Flags);
    }
    else {
        LogWarning("Failed to load file %s", Path);
        // TODO(abiab): this doesn't actually create a texture
        unsigned char BackupData = 255;
        T = ZenLoadTexture(&BackupData, 1, 1, 4, ZEN_TEXTURE_NEAREST);
    }
    
    stbi_image_free(Data);
    return T;
}


internal void
ZenUnloadTexture(texture * Texture) {
    glDeleteTextures(1, &Texture->ID);
    MemorySet(Texture, 0, sizeof(texture));
}

internal b8
ZenIsTextureValid(texture * Texture) {
    return Texture->ID != 0;
}
