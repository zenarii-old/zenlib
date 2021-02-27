//
// ~Platform Independant OpenGL functions
//
internal u32
Zen3DOpenGLLoadShader(const char * Name, const char * VertexSource, const char * FragmentSource) {
    Assert(VertexSource && FragmentSource);
    
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
        //Platform->Error("Zen3D OpenGL - Vertex Shader", InfoLog);
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
        //Platform->Error("Zen3D OpenGL - ShaderProgram", InfoLog);
    }
    
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
    
    // NOTE(Abi): Attach uniforms
    i32 idx = glGetUniformBlockIndex(Shader, "Matrices");
    if(idx != GL_INVALID_INDEX)
        glUniformBlockBinding(Shader, idx, ZEN3D_UNIFORM_MATRICES);
    idx = glGetUniformBlockIndex(Shader, "Lights");
    if(idx != GL_INVALID_INDEX)
        glUniformBlockBinding(Shader, idx, ZEN3D_UNIFORM_LIGHTS);
    
    return Shader;
}

internal shader
Zen3DLoadCustomShader(char * Name, char * VertexPath, char * FragmentPath) {
    char * VertexSource = Platform->LoadFile(VertexPath, 1);
    char * FragmentSource = Platform->LoadFile(FragmentPath, 1);
    shader Shader = Zen3DOpenGLLoadShader(Name, VertexSource, FragmentSource);
    
    glUseProgram(Shader);
#if 1
    char SamplerName[32];
    for(i32 i = 0; i < 10; ++i) {
        sprintf(SamplerName, "Texture%d", i);
        GLint Location = glGetUniformLocation(Shader, SamplerName);
        if(Location != -1) glUniform1i(Location, i);
    }
#else
    GLint Loc1 = glGetUniformLocation(Shader, "Texture0");
    GLint Loc2 = glGetUniformLocation(Shader, "Texture1");
    Assert(Loc1 != -1);
    Assert(Loc2 != -1);
    glUniform1i(Loc1, 0);
    glUniform1i(Loc2, 1);
#endif
    
    return Shader;
}

//
// ~State
//

internal void
Zen3DSetActiveCamera(camera * Camera) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_SET_CAMERA;
    Zen3D->ActiveRequest->Data = Camera;
}

internal void
Zen3DSetActiveSun(sun * Sun) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_SET_SUN;
    Zen3D->ActiveRequest->Data = Sun;
}

internal void
Zen3DEnable(zen3d_capability Cap) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_ENABLE;
    Zen3D->ActiveRequest->Data = (void *)Cap;
}

internal void
Zen3DDisable(zen3d_capability Cap) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_DISABLE;
    Zen3D->ActiveRequest->Data = (void *)Cap;
}

internal void
Zen3DPolyMode(zen3d_poly_mode Mode) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_POLYMODE;
    Zen3D->ActiveRequest->Data = (void *)Mode;
}

//
// ~Textures
//

internal void
Zen3DPushTexturedQuadTint(v3 p0, v3 p1, v3 p2, v3 p3, texture *Texture, v4 Source, v4 Colour) {
    Assert((Zen3D->Textures.AllocPos/Zen3D->Textures.Size) + 6 < Zen3D->Textures.Max);
    Assert(ZenIsTextureValid(Texture));
    
    if(!Zen3D->ActiveRequest || Zen3D->ActiveRequest->Type != ZEN3D_REQUEST_RGBA || Texture != Zen3D->ActiveRequest->Texture) {
        Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
        Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_TEXTURE;
        
        Zen3D->ActiveRequest->Data = Zen3D->Textures.Memory + Zen3D->Textures.AllocPos;
        Zen3D->ActiveRequest->DataLength = 0;
        Zen3D->ActiveRequest->Texture = Texture;
    }
    
    // NOTE(Abi): normalise texture coords
    Source.x /= Texture->Width;  Source.Width  /= Texture->Width;
    Source.y /= Texture->Height; Source.Height /= Texture->Height;
    
    GLubyte * Data = Zen3D->Textures.Memory + Zen3D->Textures.AllocPos;
    {
        *(v3 *)((f32 *)(Data)+0)  = p0;
        *(v4 *)((f32 *)(Data)+3)  = Colour;
        *(v2 *)((f32 *)(Data)+7)  = v2(Source.x + Source.Width, Source.y + Source.Height);
        
        *(v3 *)((f32 *)(Data)+9)  = p1;
        *(v4 *)((f32 *)(Data)+12) = Colour;
        *(v2 *)((f32 *)(Data)+16)  = v2(Source.x + Source.Width, Source.y);
        
        *(v3 *)((f32 *)(Data)+18) = p3;
        *(v4 *)((f32 *)(Data)+21) = Colour;
        *(v2 *)((f32 *)(Data)+25)  = v2(Source.x, Source.y + Source.Height);
        
        *(v3 *)((f32 *)(Data)+27) = p1;
        *(v4 *)((f32 *)(Data)+30) = Colour;
        *(v2 *)((f32 *)(Data)+34)  = v2(Source.x + Source.Width, Source.y);
        
        *(v3 *)((f32 *)(Data)+36) = p2;
        *(v4 *)((f32 *)(Data)+39) = Colour;
        *(v2 *)((f32 *)(Data)+43)  = v2(Source.x, Source.y);
        
        *(v3 *)((f32 *)(Data)+45) = p3;
        *(v4 *)((f32 *)(Data)+48) = Colour;
        *(v2 *)((f32 *)(Data)+52)  = v2(Source.x, Source.y + Source.Height);
    }
    
    Zen3D->Textures.AllocPos += Zen3D->Textures.Stride * 6;
    Zen3D->ActiveRequest->DataLength += Zen3D->Textures.Stride * 6;
}

internal void
Zen3DPushTexturedQuad(v3 p0, v3 p1, v3 p2, v3 p3, texture * Texture, v4 Source) {
    v4 White = v4(1, 1, 1, 1);
    Zen3DPushTexturedQuadTint(p0, p1, p2, p3, Texture, Source, White);
}

//
// ~Meshes
//
// TODO(Abi): choose from unlit colours, unlit texture, colour + texture + lighting
internal static_mesh
Zen3DStaticMeshFromData(u32 Count, v3 * Vertices, v4 * Colours, v3 * Normals, v2 * UVs) {
    Assert(Vertices && Colours && Normals && UVs);
    
    u32 FLOAT_NUM = 12;
    
    static_mesh Mesh = {0};
    {
        glGenVertexArrays(1, &Mesh.VAO);
        glBindVertexArray(Mesh.VAO);
        
        glGenBuffers(1, &Mesh.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Mesh.VBO);
        
        OpenGLAddFloatAttribute(0, 3, FLOAT_NUM, 0);
        OpenGLAddFloatAttribute(1, 4, FLOAT_NUM, 3);
        OpenGLAddFloatAttribute(2, 3, FLOAT_NUM, 7);
        OpenGLAddFloatAttribute(3, 2, FLOAT_NUM, 10);
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
        
        Data[i * FLOAT_NUM + k++] = Normals[i].x;
        Data[i * FLOAT_NUM + k++] = Normals[i].y;
        Data[i * FLOAT_NUM + k++] = Normals[i].z;
        
        Data[i * FLOAT_NUM + k++] = UVs[i].x;
        Data[i * FLOAT_NUM + k++] = UVs[i].y;
    }
    
    Mesh.VerticesCount = Count;
    Mesh.Textures      = &Zen3D->White;
    Mesh.TextureCount  = 1;
    Mesh.Shader        = Zen3D->Shaders[ZEN3D_SHADER_LIGHTING];
    
    glBufferData(GL_ARRAY_BUFFER, Count * FLOAT_NUM * sizeof(f32), Data, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    return Mesh;
}

internal void
Zen3DUnloadStaticMesh(static_mesh * Mesh) {
    glDeleteVertexArrays(1, &Mesh->VAO);
    glDeleteBuffers(1, &Mesh->VBO);
    Mesh->VerticesCount = 0;
}

internal void
Zen3DPushStaticMesh(static_mesh * Mesh) {
    Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
    Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_STATIC_MESH;
    Zen3D->ActiveRequest->Data = Mesh;
}

//
// ~Textures
//

internal void
Zen3DPushTexturedMesh(static_mesh * Mesh) {
    
}

//
// ~Shape Drawing
//

internal void
Zen3DPushLine(v3 p0, v3 p1, v4 Colour) {
    Assert((Zen3D->Shapes.AllocPos/Zen3D->Shapes.Size) + 6 < Zen3D->Shapes.Max);
    
    if(!Zen3D->ActiveRequest || Zen3D->ActiveRequest->Type != ZEN3D_REQUEST_RGBA) {
        Zen3D->ActiveRequest = &Zen3D->Requests[Zen3D->RequestCount++];
        Zen3D->ActiveRequest->Type = ZEN3D_REQUEST_LINE;
        
        Zen3D->ActiveRequest->Data = Zen3D->Shapes.Memory + Zen3D->Shapes.AllocPos;
        Zen3D->ActiveRequest->DataLength = 0;
    }
    
    GLubyte * Data = Zen3D->Shapes.Memory + Zen3D->Shapes.AllocPos;
    {
        *(v3 *)((f32 *)(Data)+0)  = p0;
        *(v4 *)((f32 *)(Data)+3)  = Colour;
        
        *(v3 *)((f32 *)(Data)+7)  = p1;
        *(v4 *)((f32 *)(Data)+10) = Colour;
    }
    Zen3D->Shapes.AllocPos += Zen3D->Shapes.Stride * 2;
    Zen3D->ActiveRequest->DataLength += Zen3D->Shapes.Stride * 2;
}

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
    
#if 0
    {
        Zen3D->Shapes.Stride = sizeof(f32) * 7;
        // NOTE(Abi): Since made of triangles.
        Zen3D->Shapes.Size   = 3 * Zen3D->Shapes.Stride;
        Zen3D->Shapes.Max    = 1024;
        
        Zen3D->Shapes.Memory = MemoryArenaAlloc(Arena, Zen3D->Shapes.Size * Zen3D->Shapes.Max);
    }
#endif
#define ZEN3DBATCH(name, stride, size, max) \
{ \
Zen3D->name.Stride = stride; \
Zen3D->name.Size   = size; \
Zen3D->name.Max    = max; \
    \
Zen3D->name.Memory = MemoryArenaAlloc(Arena, Zen3D->name.Size * Zen3D->name.Max); \
}
#include "zen3d_batch_data_types.inc"
    
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
    
    // NOTE(Abi): Texture Data
    {
        glGenVertexArrays(1, &Zen3D->Textures.VAO);
        glBindVertexArray(Zen3D->Textures.VAO);
        
        glGenBuffers(1, &Zen3D->Textures.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Textures.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen3D->Textures.Max * Zen3D->Textures.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 3, 9, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 9, 3);
        // NOTE(Abi): UV data
        OpenGLAddFloatAttribute(2, 2, 9, 7);
        
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Generate Uniforms
    glGenBuffers(1, &Zen3D->Uniformbuffers[ZEN3D_UNIFORM_MATRICES]);
    glBindBuffer(GL_UNIFORM_BUFFER, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_MATRICES]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(f32) * 16, 0, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, ZEN3D_UNIFORM_MATRICES, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_MATRICES]); 
    
    glGenBuffers(1, &Zen3D->Uniformbuffers[ZEN3D_UNIFORM_LIGHTS]);
    glBindBuffer(GL_UNIFORM_BUFFER, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_LIGHTS]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(sun) + sizeof(v3), 0, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, ZEN3D_UNIFORM_LIGHTS, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_LIGHTS]);
    
    
#include "shaders/generated_opengl_shaders.inc"
    for(i32 i = 0; i < ZEN3D_SHADER_COUNT; ++i) {
        fprintf(stderr, "[Zen3D] Loading '%s' shader.\n", ShaderInfo[i].Name);
        Zen3D->Shaders[i] = Zen3DOpenGLLoadShader(ShaderInfo[i].Name, ShaderInfo[i].VertexSource, ShaderInfo[i].FragmentSource);
    }
    
    Zen3D->Framebuffer = OpenGLCreateFramebuffer(Platform->ScreenWidth, Platform->ScreenHeight);
    
    unsigned char White = 255;
    Zen3D->White = ZenLoadTexture(&White, 1, 1, 4, ZEN_TEXTURE_NEAREST);
    
    // TODO(abi): probably set a default sun so that the lighting isn't fucked?
}

internal void
Zen3DBeginFrame() {
#define ZEN3DBATCH(name, stride, size, max) Zen3D->name.AllocPos = 0;
#include "zen3d_batch_data_types.inc"
    
    Zen3D->RequestCount = 0;
    Zen3D->ActiveRequest = 0;
    
    {
        Zen3D->RendererWidth  = Platform->ScreenWidth;
        Zen3D->RendererHeight = Platform->ScreenHeight;
    }
    // TEMP(Abi);
    OpenGLBindFramebuffer(&Zen3D->Framebuffer);
    glClearColor(0.53, 0.81, 0.92, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO(Abi): figure out where to put this
    OpenGLBindFramebuffer(0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

internal void
Zen3DEndFrame() {
    // TEMP(Abi): should be able to remove
    OpenGLBindFramebuffer(&Zen3D->Framebuffer);
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
            
            case ZEN3D_REQUEST_TEXTURE: {
                glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_TEXTURE]);
                glBindVertexArray(Zen3D->Textures.VAO);
                {
                    glBindTexture(GL_TEXTURE_2D, Request->Texture->ID);
                    glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Textures.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Request->DataLength, Request->Data);
                    i32 Count = Request->DataLength/Zen3D->Textures.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            case ZEN3D_REQUEST_LINE: {
                glUseProgram(Zen3D->Shaders[ZEN3D_SHADER_RGBA]);
                glBindVertexArray(Zen3D->Shapes.VAO);
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Zen3D->Shapes.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Request->DataLength, Request->Data);
                    i32 Count = Request->DataLength/Zen3D->Shapes.Stride;
                    glDrawArrays(GL_LINES, 0, Count);
                }
                glBindVertexArray(0);
            }
            
            case ZEN3D_REQUEST_SET_CAMERA: {
                camera * Camera = Request->Data;
                f32 Ratio = Zen3D->RendererWidth/Zen3D->RendererHeight;
                matrix4x4 Projection = PerspectiveMatrix(Camera->fov, Ratio, 0.1f, 100.f);
                matrix4x4 View;
                if(Camera->Mode == CAMERA_MODE_LOOK_AT) {
                    View = LookAt(Camera->Position, Camera->Target, v3(0.f, 1.f, 0.f));
                }
                else {
                    View = LookAt(Camera->Position, 
                                  AddV3(Camera->Position, Camera->Target), 
                                  v3(0.f, 1.f, 0.f));
                }
                // NOTE(Abi): Since OpenGL uses column-major matrices, have to take transpose.
                matrix4x4 VP = TransposeMatrix(MultM4M4(Projection, View));
                glBindBuffer(GL_UNIFORM_BUFFER, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_MATRICES]);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &VP.Elements[0][0]); 
                glBindBuffer(GL_UNIFORM_BUFFER, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_LIGHTS]);
                glBufferSubData(GL_UNIFORM_BUFFER, sizeof(sun), sizeof(v3), &Camera->Position);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            } break;
            
            case ZEN3D_REQUEST_POLYMODE: {
                zen3d_poly_mode Mode = (uintptr_t)Request->Data;
                switch(Mode) {
                    case ZEN3D_WIREFRAME: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  break;
                    case ZEN3D_POINTS:    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
                    case ZEN3D_FILLED:    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  break;
                }
            } break;
            
            case ZEN3D_REQUEST_ENABLE: {
                zen3d_capability Cap = (uintptr_t)Request->Data;
                switch(Cap) {
                    case ZEN3D_DEPTH: glEnable(GL_DEPTH_TEST); break;
                }
            } break;
            
            case ZEN3D_REQUEST_DISABLE: {
                zen3d_capability Cap = (uintptr_t)Request->Data;
                switch(Cap) {
                    case ZEN3D_DEPTH: glDisable(GL_DEPTH_TEST); break;
                }
            } break;
            
            case ZEN3D_REQUEST_SET_SUN: {
                sun * Sun = Request->Data;
                glBindBuffer(GL_UNIFORM_BUFFER, Zen3D->Uniformbuffers[ZEN3D_UNIFORM_LIGHTS]);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(sun), Sun); 
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            } break;
            
            // TODO(Abi): No batching for static objects yet.
            case ZEN3D_REQUEST_STATIC_MESH: {
                static_mesh * Mesh = Request->Data; // TODO(Abi): check this works when deleting mesh.
                glUseProgram(Mesh->Shader);
                
                for(i32 i = 0; i < Mesh->TextureCount; ++i) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, Mesh->Textures[i].ID);
                }
                glActiveTexture(GL_TEXTURE0);
                
                glBindVertexArray(Mesh->VAO);
                glBindTexture(GL_TEXTURE_2D, Mesh->Textures->ID);
                glDrawArrays(GL_TRIANGLES, 0, Mesh->VerticesCount);
                glBindVertexArray(0);
                //glActiveTexture(GL_TEXTURE0);
            } break;
            
            default: Assert("[Zen3D] Request had an invalid type" == 0);
        }
    }
    
#ifndef ZEN2D
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Zen3D->Framebuffer.ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glBlitFramebuffer(0,
                      0,
                      Platform->ScreenWidth,
                      Platform->ScreenHeight,
                      0,
                      0,
                      Platform->ScreenWidth,
                      Platform->ScreenHeight,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
#else
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Zen3D->Framebuffer.ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Zen2D->Framebuffer[ZEN2D_FBO_MAIN].ID);
    
    glBlitFramebuffer(0,
                      0,
                      Platform->ScreenWidth,
                      Platform->ScreenHeight,
                      0,
                      0,
                      Platform->ScreenWidth,
                      Platform->ScreenHeight,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
#endif
    glDisable(GL_DEPTH_TEST);
}