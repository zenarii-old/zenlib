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

internal void
Zen3DOpenGLAddFloatAttribute(i32 ID, u32 Count, u32 Stride, u32 Offset) {
    glVertexAttribPointer(ID, Count, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void *)(Offset*sizeof(f32)));
    glEnableVertexAttribArray(ID);
}

internal void
Zen3DPushQuad(v3 p0, v3 p1, v3 p2, v3 p3) {
    
}

internal void
Zen3DInit(memory_arena * Arena) {
#ifndef ZEN2D
    // TODO(Abi): Load opengl functions
#endif
    
    Zen3D->Shapes.Stride = sizeof(f32) * 7;
    // NOTE(Abi): Since made of triangles.
    Zen3D->Shapes.Size   = 3 * Zen3D->Shapes.Stride;
    Zen3D->Shapes.Max    = 1024;
    
    Zen3D->Shapes.Memory = MemoryArenaAlloc(Arena, Zen3D->Shapes.Size * Zen3D->Shapes.Max);
    
    // NOTE(Abi): Shape Data
    {
        glGenVertexArrays(1, &Zen3D->Shapes.VAO);
        glBindVertexArray(Zen3D->Shapes.VAO);
        
        glGenBuffers(1, &Zen3D->Shapes.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Shapes.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen3D->Shapes.Size * Zen3D->Shapes.Max, 0, GL_DYNAMIC_DRAW);
        
        Zen3DOpenGLAddFloatAttribute(0, 3, 7, 0);
        Zen3DOpenGLAddFloatAttribute(0, 4, 7, 3);
    }
}

internal void
Zen3DBeginFrame() {
    
}

internal void
Zen3DEndFrame() {
    glEnable(GL_DEPTH);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    //BIND FRAMEBUFFER,
    
    glDisable(GL_DEPTH);
}