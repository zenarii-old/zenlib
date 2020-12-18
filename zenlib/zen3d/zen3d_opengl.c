//
// ~Platform Independant OpenGL functions
//
// TODO(Abi): unify this into zencore_opengl.c
internal u32
Zen3DOpenGLLoadShader(const char * Name, const char * VertexSource, const char * FragmentSource) {
    
    
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
        Platform->Error("Zen3D OpenGL - Vertex Shader", InfoLog);
    }
    
    u32 Fragment = 0;
    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, (const GLchar * const *)&FragmentSource, 0);
    glCompileShader(Fragment);
    
    glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SuccessfulCompilation);
    if(!SuccessfulCompilation) {
        glGetShaderInfoLog(Fragment, 512, 0, InfoLog);
        LogError("Failed to compile fragment shader %s.\n%s", Name, InfoLog);
        Platform->Error("Zen3D OpenGL - Fragment Shader", InfoLog);
    }
    
    Shader = glCreateProgram();
    glAttachShader(Shader, Vertex);
    glAttachShader(Shader, Fragment);
    glLinkProgram(Shader);
    
    glGetProgramiv(Shader, GL_LINK_STATUS, &SuccessfulCompilation);
    if(!SuccessfulCompilation) {
        glGetProgramInfoLog(Shader, 512, 0, InfoLog);
        LogError("Failed to compile shader program %s.\n%s", Name, InfoLog);
        Platform->Error("Zen3D OpenGL - ShaderProgram", InfoLog);
    }
    
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
    
    return Shader;
}
//
// ~Uniform functions
//
// TODO(Abi): Uniform buffer object for view and projection (could mult before sending?)

//
// ~Shape Drawing
//

// TODO(Abi): Figure out the required order of p0, p1, p2, p3
internal void
Zen3DPushQuad(v3 p0, v3 p1, v3 p2, v3 p3, v4 Colour) {
    Assert((Zen3D->Shapes.AllocPos/Zen3D->Shapes.Size) + 6 < Zen3D->Shapes.Max);
    
    if(!Zen3D->ActiveRequest || Zen3D->ActiveRequest->Type != ZEN3D_REQUEST_STANDARD) {
        Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
        Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_STANDARD;
        
        Zen3D->ActiveRequest->Data = Zen3D->Shapes.Memory + Zen3D->Shapes.AllocPos;
        Zen3D->ActiveRequest->DataLength = 0;
    }
    
    // TODO(Abi): Get Matrices from the camera view and matrix
    GLubyte * Data = Zen3D->Shapes.Memory + Zen3D->Shapes.AllocPos;
    {
        *(v3 *)((f32 *)(Data)+0)  = p0;
        *(v4 *)((f32 *)(Data)+3)  = Colour;
        
        *(v3 *)((f32 *)(Data)+7)  = p1;
        *(v4 *)((f32 *)(Data)+10) = Colour;
        
        *(v3 *)((f32 *)(Data)+14) = p3;
        *(v4 *)((f32 *)(Data)+17) = Colour;
        
        *(v3 *)((f32 *)(Data)+21) = p1;
        *(v4 *)((f32 *)(Data)+24) = Colour;
        
        *(v3 *)((f32 *)(Data)+28) = p2;
        *(v4 *)((f32 *)(Data)+31) = Colour;
        
        *(v3 *)((f32 *)(Data)+35) = p3;
        *(v4 *)((f32 *)(Data)+38) = Colour;
    }
    Zen3D->Shapes.AllocPos += Zen3D->Shapes.Stride * 6;
    Zen3D->ActiveRequest->DataLength += Zen3D->Shapes.Stride * 6;
}

//
// ~
//

internal void
Zen3DInit(memory_arena * Arena) {
    {
        Zen3D->Shapes.Stride = sizeof(f32) * 7;
        // NOTE(Abi): Since made of triangles.
        Zen3D->Shapes.Size   = 3 * Zen3D->Shapes.Stride;
        Zen3D->Shapes.Max    = 1024;
        
        Zen3D->Shapes.Memory = MemoryArenaAlloc(Arena, Zen3D->Shapes.Size * Zen3D->Shapes.Max);
    }
    
    // NOTE(Abi): Shape Data
    {
        glGenVertexArrays(1, &Zen3D->Shapes.VAO);
        glBindVertexArray(Zen3D->Shapes.VAO);
        
        glGenBuffers(1, &Zen3D->Shapes.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Shapes.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen3D->Shapes.Max * Zen3D->Shapes.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 3, 7, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 7, 3);
        glBindVertexArray(0);
    }
    
    char * VSource, * FSource;
    VSource = Platform->LoadFile("vert.glsl", 1);
    FSource = Platform->LoadFile("frag.glsl", 1);
    
    Zen3D->Shaders[0] = Zen3DOpenGLLoadShader("text", VSource, FSource);
    //Log("\n%s\n---\n%s", VSource, FSource);
}

internal void
Zen3DBeginFrame() {
    Zen3D->Shapes.AllocPos = 0;
    Zen3D->RequestCount = 0;
    Zen3D->ActiveRequest = 0;
    // TODO(Abi): Clear framebuffer colour/depth
    
    // TEMP(Abi);
    
    
}

internal void
Zen3DEndFrame() {
    Assert(Zen3D->RequestCount < ZEN3D_MAX_REQUESTS);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    for(i32 idx = 0; idx < Zen3D->RequestCount; ++idx) {
        zen3d_request * Request = &Zen3D->Requests[idx];
        
        switch(Request->Type) {
            case ZEN3D_REQUEST_STANDARD: {
                glUseProgram(Zen3D->Shaders[0]);
                glBindVertexArray(Zen3D->Shapes.VAO);
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Shapes.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Request->DataLength, Request->Data);
                    i32 Count = Request->DataLength/Zen3D->Shapes.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            default: Assert("[Zen3D] Request had an invalid type" == 0);
        }
    }
    
    glDisable(GL_DEPTH_TEST);
}