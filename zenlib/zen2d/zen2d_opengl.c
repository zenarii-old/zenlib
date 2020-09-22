internal void
Zen2DOpenGLLoadAllFunctions(void) {
#define OPENGLPROC(function, type) gl##function = (PFNGL##type##PROC)Platform->OpenGLLoadProcedure("gl" #function);
#include "zen2d_opengl_proc_list.inc"
}

internal u32
Zen2DOpenGLLoadShader(const char * Name, const char * VertexSource, const char * FragmentSource) {
    u32 Shader = 0;
    b32 SuccessfulCompilation = 0;
    char InfoLog[512] = {0};
    
    u32 Vertex = 0;
    Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, (const GLchar * const *)&VertexSource, 0);
    glCompileShader(Vertex);
    
    glGetShaderiv(Vertex, GL_COMPILE_STATUS, &SuccessfulCompilation);
    if(!SuccessfulCompilation) {
        glGetShaderInfoLog(Vertex, 512, 0, InfoLog);
        LogError("Failed to compile vertex shader %s.\n%s", Name, InfoLog);
        Platform->Error("Zen2D OpenGL - Vertex Shader", InfoLog);
    }
    
    u32 Fragment = 0;
    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, (const GLchar * const *)&FragmentSource, 0);
    glCompileShader(Fragment);
    
    glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SuccessfulCompilation);
    if(!SuccessfulCompilation) {
        glGetShaderInfoLog(Fragment, 512, 0, InfoLog);
        LogError("Failed to compile fragment shader %s.\n%s", Name, InfoLog);
        Platform->Error("Zen2D OpenGL - Fragment Shader", InfoLog);
    }
    
    Shader = glCreateProgram();
    glAttachShader(Shader, Vertex);
    glAttachShader(Shader, Fragment);
    glLinkProgram(Shader);
    
    glGetProgramiv(Shader, GL_LINK_STATUS, &SuccessfulCompilation);
    if(!SuccessfulCompilation) {
        glGetProgramInfoLog(Shader, 512, 0, InfoLog);
        LogError("Failed to compile shader program %s.\n%s", Name, InfoLog);
        Platform->Error("Zen2D OpenGL - ShaderProgram", InfoLog);
    }
    
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
    
    return Shader;
}

internal void
Zen2DOpenGLAddFloatAttribute(i32 ID, u32 Count, u32 Stride, u32 Offset) {
    glVertexAttribPointer(ID, Count, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void *)(Offset*sizeof(f32)));
    glEnableVertexAttribArray(ID);
}

#define RectMax (1024)
#define RectStride (6 * sizeof(f32))
#define RectSize (6 * RectStride)

global u32 RectVAO, RectVBO;
global unsigned char RectData[RectMax * RectSize];
global u32 RectAllocPos = 0;

global zen2d_batch Batches[ZEN2D_MAX_BATCHES] = {0};
global u32 BatchesCount = 0;

internal void
Zen2DInit(memory_arena * Arena) {
    Zen2DOpenGLLoadAllFunctions();
    
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenVertexArrays(1, &Zen2D->GeneralVAO);
    
#include "shaders/generated_opengl_shaders.inc"
    for(int i = 0; i < sizeof(ShaderInfo)/sizeof(ShaderInfo[0]); ++i) {
        fprintf(stderr, "[Zen2D] Loading '%s' shader.\n", ShaderInfo[i].Name);
        Zen2D->Shaders[i] = Zen2DOpenGLLoadShader(ShaderInfo[i].Name, ShaderInfo[i].VertexSource, ShaderInfo[i].FragmentSource);
    }
    
    {
        glGenVertexArrays(1, &RectVAO);
        glBindVertexArray(RectVAO);
        
        glGenBuffers(1, &RectVBO);
        glBindBuffer(GL_ARRAY_BUFFER, RectVBO);
        glBufferData(GL_ARRAY_BUFFER, RectMax * RectSize, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        Zen2DOpenGLAddFloatAttribute(0, 2, 6, 0);
        // NOTE(Abi): Colour data
        Zen2DOpenGLAddFloatAttribute(1, 4, 6, 2);
    }
}

internal void
Zen2DPushRectVertices(v4 Rect, v4 Colour00, v4 Colour01, v4 Colour10, v4 Colour11){
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_RECTS) {
        Zen2D->ActiveBatch = &Batches[BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_RECTS;
        Zen2D->ActiveBatch->Data = RectData + RectAllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // NOTE(Abi): Convert from the screen coordinates to opengl ones
    {
        Rect.x *= 2.f / Zen2D->RendererWidth;  Rect.x -= 1;
        Rect.y *= 2.f / Zen2D->RendererHeight; Rect.y -= 1;
        Rect.z *= 2.f / Zen2D->RendererWidth;  
        Rect.w *= 2.f / Zen2D->RendererHeight; 
    }
    
    GLubyte * Data = RectData + RectAllocPos;
    {
        ((v2 *)Data)[0]  = v2(Rect.x, Rect.y);
        ((v2 *)Data)[1]  = v2(Colour00.x, Colour00.y);
        ((v2 *)Data)[2]  = v2(Colour00.z, Colour00.w);
        
        ((v2 *)Data)[3]  = v2(Rect.x + Rect.Width, Rect.y);
        ((v2 *)Data)[4]  = v2(Colour10.x, Colour10.y);
        ((v2 *)Data)[5]  = v2(Colour10.z, Colour10.w);
        
        ((v2 *)Data)[6]  = v2(Rect.x, Rect.y + Rect.Height);
        ((v2 *)Data)[7]  = v2(Colour01.x, Colour01.y);
        ((v2 *)Data)[8]  = v2(Colour01.z, Colour01.w);
        
        
        ((v2 *)Data)[9]  = v2(Rect.x + Rect.Width, Rect.y + Rect.Height);
        ((v2 *)Data)[10] = v2(Colour11.x, Colour11.y);
        ((v2 *)Data)[11] = v2(Colour11.z, Colour11.w);
        
        ((v2 *)Data)[12] = v2(Rect.x + Rect.Width, Rect.y);
        ((v2 *)Data)[13] = v2(Colour10.x, Colour10.y);
        ((v2 *)Data)[14] = v2(Colour10.z, Colour10.w);
        
        ((v2 *)Data)[15] = v2(Rect.x, Rect.y + Rect.Height);
        ((v2 *)Data)[16] = v2(Colour01.x, Colour01.y);
        ((v2 *)Data)[17] = v2(Colour01.z, Colour01.w);
    }
    RectAllocPos += RectSize;
    Zen2D->ActiveBatch->DataLength += RectSize;
}

internal void
Zen2DPushRect(v4 Rect, v4 Colour) {
    Zen2DPushRectVertices(Rect, Colour, Colour, Colour, Colour);
}

internal void
Zen2DBeginFrame() {
    RectAllocPos = 0;
    Zen2D->ActiveBatch = 0;
    BatchesCount = 0;
    // NOTE(Abi): Transfer Data from platform
    {
        Zen2D->RendererWidth  = Platform->ScreenWidth;
        Zen2D->RendererHeight = Platform->ScreenHeight;
    }
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void
Zen2DEndFrame() {
    for(i32 i = 0; i < BatchesCount; ++i) {
        zen2d_batch * Batch = &Batches[i];
        switch(Batch->Type) {
            case ZEN2D_BATCH_RECTS: {
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_RECTANGLES]);
                glBindVertexArray(RectVAO);
                glBindBuffer(GL_ARRAY_BUFFER, RectVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                glDrawArrays(GL_TRIANGLES, 0, Batch->DataLength/RectStride);
            } break;
            default: break;
        }
    }
}