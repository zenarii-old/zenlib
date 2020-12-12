
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
Zen3DInit(memory_arena * Arena) {
    char * VSource, * FSource;
    VSource = 
        "#version 330 core\n"\
        "layout(location = 0) in vec3 inPosition;\n"\
        "layout(location = 1) in vec4 inColour;\n"\
        "out vec4 fColour;\n"\
        "void main() {\n"\
        "gl_Position = vec4(inPosition, 1.0);\n"\
        "fColour = inColour;\n"\
        "}";
    FSource = 
        "#version 330 core\n"\
        "in vec4 fColour;\n"\
        "out vec4 FragColour;\n"\
        "void main() {\n"\
        "FragColour = fColour;\n"\
        "}";
    
    Zen3D->Shader = Zen3DOpenGLLoadShader("Test", VSource, FSource);
}

internal void
Zen3DBeginFrame() {
    
}

internal void
Zen3DEndFrame() {
    glEnable(GL_DEPTH);
    // TODO(Abi): Clear the framebuffer dpeth/colour stuff
    
    
    glDisable(GL_DEPTH);
}