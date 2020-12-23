//
// ~Platform Independant OpenGL functions
//
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
// ~
//

internal void
Zen3DSetActiveCamera(camera * Camera) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_SET_CAMERA;
    Zen3D->ActiveRequest->Data = Camera;
}

//
// ~Meshes
//

internal static_mesh
Zen3DStaticMeshFromData(u32 Count, v3 * Vertices, v4 * Colours) {
    u32 FLOAT_NUM = 7;
    
    static_mesh Mesh = {0};
    {
        glGenVertexArrays(1, &Mesh.VAO);
        glBindVertexArray(Mesh.VAO);
        
        glGenBuffers(1, &Mesh.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Mesh.VBO);
        
        OpenGLAddFloatAttribute(0, 3, FLOAT_NUM, 0);
        OpenGLAddFloatAttribute(1, 4, FLOAT_NUM, 3);
    }
    
    // NOTE(Abi): Copy the data across
    f32 * Data = MemoryArenaAlloc(&Platform->ScratchArena, FLOAT_NUM * sizeof(f32) * Count);
    for(i32 i = 0; i < Count; ++i) {
        i32 k = 0;
        Data[i * FLOAT_NUM + k++] = Vertices[i].x;
        Data[i * FLOAT_NUM + k++] = Vertices[i].y;
        Data[i * FLOAT_NUM + k++] = Vertices[i].z;
        
        Data[i * FLOAT_NUM + k++] = Colours[i].x;
        Data[i * FLOAT_NUM + k++] = Colours[i].y;
        Data[i * FLOAT_NUM + k++] = Colours[i].z;
        Data[i * FLOAT_NUM + k++] = Colours[i].w;
    }
    
    Mesh.VerticesCount = Count;
    
    
    glBufferData(GL_ARRAY_BUFFER, Count * FLOAT_NUM * sizeof(f32), Data, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    return Mesh;
}

internal void
Zen3DPushStaticMesh(static_mesh * Mesh) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_STATIC_MESH;
    Zen3D->ActiveRequest->Data = Mesh;
}

//
// ~Shape Drawing
//

// TODO(Abi): Figure out the required order of p0, p1, p2, p3
internal void
Zen3DPushQuad(v3 p0, v3 p1, v3 p2, v3 p3, v4 Colour) {
    Assert((Zen3D->Shapes.AllocPos/Zen3D->Shapes.Size) + 6 < Zen3D->Shapes.Max);
    
    if(!Zen3D->ActiveRequest || Zen3D->ActiveRequest->Type != ZEN3D_REQUEST_RGBA) {
        Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
        Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_RGBA;
        
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
    glEnable(GL_BLEND);
    
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
    
#include "shaders/generated_opengl_shaders.inc"
    for(i32 i = 0; i < ZEN3D_SHADER_COUNT; ++i) {
        fprintf(stderr, "[Zen3D] Loading '%s' shader.\n", ShaderInfo[i].Name);
        Zen3D->Shaders[i] = Zen3DOpenGLLoadShader(ShaderInfo[i].Name, ShaderInfo[i].VertexSource, ShaderInfo[i].FragmentSource);
    }
    
    fprintf(stderr, "[Zen3D] Loaded\n");
}

internal void
Zen3DBeginFrame() {
    Zen3D->Shapes.AllocPos = 0;
    Zen3D->RequestCount = 0;
    Zen3D->ActiveRequest = 0;
    // TODO(Abi): Clear framebuffer colour/depth
    
    {
        Zen3D->RendererWidth  = Platform->ScreenWidth;
        Zen3D->RendererHeight = Platform->ScreenHeight;
    }
    // TEMP(Abi);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

internal void
Zen3DEndFrame() {
    // TEMP(Abi): delete when have a uniform buffer
    {
        glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_RGBA]);
    }
    
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Assert(Zen3D->RequestCount < ZEN3D_MAX_REQUESTS);
    for(i32 idx = 0; idx < Zen3D->RequestCount; ++idx) {
        zen3d_request * Request = &Zen3D->Requests[idx];
        
        switch(Request->Type) {
            case ZEN3D_REQUEST_RGBA: {
                glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_RGBA]);
                glBindVertexArray(Zen3D->Shapes.VAO);
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Shapes.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Request->DataLength, Request->Data);
                    i32 Count = Request->DataLength/Zen3D->Shapes.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            case ZEN3D_REQUEST_SET_CAMERA: {
                camera * Camera = Request->Data;
                f32 Ratio = Zen3D->RendererWidth/Zen3D->RendererHeight;
                matrix4x4 Projection = PerspectiveMatrix(0.5f*PI, Ratio, 0.1f, 100.f);
                matrix4x4 View;
                if(Camera->Mode == CAMERA_MODE_LOOK_AT) {
                    View = LookAt(Camera->Position, Camera->Target, v3(0.f, 1.f, 0.f));
                }
                else {
                    View = LookAt(Camera->Position, 
                                  AddV3(Camera->Position, Camera->Target), 
                                  v3(0.f, 1.f, 0.f));
                }
                matrix4x4 VP = MultM4M4(Projection, View);
                GLuint VPLocation = glGetUniformLocation(Zen3D->Shaders[0], "VP");
                glUniformMatrix4fv(VPLocation, 1, GL_TRUE, &VP.Elements[0][0]);
            } break;
            
            // TODO(Abi): No batching for static objects yet.
            case ZEN3D_REQUEST_STATIC_MESH: {
                glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_RGBA]); //mesh shader?
                static_mesh * Mesh = Request->Data; // TODO(Abi): check this works when deleting mesh.
                glBindVertexArray(Mesh->VAO);
                glDrawArrays(GL_TRIANGLES, 0, Mesh->VerticesCount);
                glBindVertexArray(0);
            } break;
            
            default: Assert("[Zen3D] Request had an invalid type" == 0);
        }
    }
#ifdef ZEN2D
    //glBindFramebuffer(GL_FRAMEBUFFER, Zen2D->Framebuffer[ZEN2D_FBO_MAIN].ID);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_FBO_BLIT]);
    //glBindTexture(GL_TEXTURE_2D, Zen3D->Framebuffer.Texture);
    //glBindVertexArray(Zen2D->FramebufferBlit.VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);
#else
    
#endif
    glDisable(GL_DEPTH_TEST);
    
}