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


internal font
Zen2DLoadFont(void * PNGData, i32 Width, i32 Height, i32 Channels, font_glyph * Glyphs, u32 GlyphCount, u32 LineHeight, u32 FontSize, u32 Base, u32 LowestChar) {
    font Font = {0};
    Font.Texture = ZenLoadTexture(PNGData, Width, Height, Channels, ZEN_TEXTURE_LINEAR);
    Font.Glyphs = Platform->HeapAlloc(sizeof(font_glyph) * GlyphCount);
    MemoryCopy(Font.Glyphs, Glyphs, GlyphCount * sizeof(font_glyph));
    Font.GlyphCount = GlyphCount;
    Font.LowestChar = LowestChar;
    Font.LineHeight = LineHeight;
    Font.Base = Base;
    Font.Size = FontSize;
    
    return Font;
}

internal void
Zen2DUnloadFont(font * Font) {
    Platform->HeapFree(Font->Glyphs);
    ZenUnloadTexture(&Font->Texture);
    MemorySet(Font, 0, sizeof(font));
}

internal b8
Zen2DIsFontValid(font * Font) {
    return ZenIsTextureValid(&Font->Texture) && Font->Glyphs;
}

internal void
Zen2DInit(memory_arena * Arena) {
    Zen2DInitCommon();
    
    glEnable(GL_BLEND);
    
    glGenVertexArrays(1, &Zen2D->GeneralVAO);
    
#include "shaders/generated_opengl_shaders.inc"
    
    for(int i = 0; i < sizeof(ShaderInfo)/sizeof(ShaderInfo[0]); ++i) {
        fprintf(stderr, "[Zen2D] Loading '%s' shader.\n", ShaderInfo[i].Name);
        Zen2D->Shaders[i] = Zen2DOpenGLLoadShader(ShaderInfo[i].Name, ShaderInfo[i].VertexSource, ShaderInfo[i].FragmentSource);
    }
    
#define ZEN2DBATCH(name, stride, size, max) \
{ \
Zen2D->name.Stride = stride; \
Zen2D->name.Size = size; \
Zen2D->name.Max = max; \
Zen2D->name.Memory = MemoryArenaAlloc(Arena, max * size); \
Zen2D->name.AllocPos = 0; \
}
#include "zen2d_batch_data_types.inc"
    
    // NOTE(Abi): Rectangle Data
    {
        glGenVertexArrays(1, &Zen2D->Rect.VAO);
        glBindVertexArray(Zen2D->Rect.VAO);
        
        glGenBuffers(1, &Zen2D->Rect.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Rect.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen2D->Rect.Max * Zen2D->Rect.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 2, 6, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 6, 2);
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Line Data
    {
        glGenVertexArrays(1, &Zen2D->Line.VAO);
        glBindVertexArray(Zen2D->Line.VAO);
        
        glGenBuffers(1, &Zen2D->Line.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Line.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen2D->Line.Max * Zen2D->Line.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 2, 6, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 6, 2);
        
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Texture Data
    {
        glGenVertexArrays(1, &Zen2D->Texture.VAO);
        glBindVertexArray(Zen2D->Texture.VAO);
        
        glGenBuffers(1, &Zen2D->Texture.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Texture.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen2D->Texture.Max * Zen2D->Texture.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 2, 8, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 8, 2);
        // NOTE(Abi): UV data
        OpenGLAddFloatAttribute(2, 2, 8, 6);
        
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Text Data
    {
        glGenVertexArrays(1, &Zen2D->Text.VAO);
        glBindVertexArray(Zen2D->Text.VAO);
        
        glGenBuffers(1, &Zen2D->Text.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Text.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen2D->Text.Max * Zen2D->Text.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position data
        OpenGLAddFloatAttribute(0, 2, 8, 0);
        // NOTE(Abi): Colour data
        OpenGLAddFloatAttribute(1, 4, 8, 2);
        // NOTE(Abi): UV data
        OpenGLAddFloatAttribute(2, 2, 8, 6);
        
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Blur Data
    {
        glGenVertexArrays(1, &Zen2D->Blur.VAO);
        glBindVertexArray(Zen2D->Blur.VAO);
        
        glGenBuffers(1, &Zen2D->Blur.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Blur.VBO);
        glBufferData(GL_ARRAY_BUFFER, Zen2D->Blur.Max * Zen2D->Blur.Size, 0, GL_DYNAMIC_DRAW);
        // NOTE(Abi): Position Data
        OpenGLAddFloatAttribute(0, 2, 2, 0);
        glBindVertexArray(0);
    }
    
    // NOTE(Abi): Framebuffer Blit Data
    {
        f32 Vertices[] = {
            -1.f, -1.f, 0.f, 0.f,
            -1.f,  1.f, 0.f, 1.f,
            1.f,  -1.f, 1.f, 0.f,
            
            -1.f,  1.f, 0.f, 1.f,
            1.f,  -1.f, 1.f, 0.f,
            1.f,   1.f, 1.f, 1.f,
        };
        
        glGenVertexArrays(1, &Zen2D->FramebufferBlit.VAO);
        glBindVertexArray(Zen2D->FramebufferBlit.VAO);
        
        glGenBuffers(1, &Zen2D->FramebufferBlit.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Zen2D->FramebufferBlit.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        
        // NOTE(Abi): Position Data
        OpenGLAddFloatAttribute(0, 2, 4, 0);
        // NOTE(Abi): UV Data
        OpenGLAddFloatAttribute(1, 2, 4, 2);
    }
    
    // NOTE(Abi): Load FBOs
    for(i32 i = 0; i < ZEN2D_FBO_COUNT; ++i) {
        Zen2D->Framebuffer[i] = OpenGLCreateFramebuffer(Platform->ScreenWidth, Platform->ScreenHeight);
    }
}

internal void
Zen2DPushRectVertices(v4 Rect, v4 Colour00, v4 Colour01, v4 Colour10, v4 Colour11) {
    Assert(Zen2D->Rect.AllocPos/Zen2D->Rect.Size < Zen2D->Rect.Max);
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_RECTS) {
        Zen2D->ActiveBatch = &Zen2D->Batches[Zen2D->BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_RECTS;
        Zen2D->ActiveBatch->Data = Zen2D->Rect.Memory + Zen2D->Rect.AllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // NOTE(Abi): Convert from the screen coordinates to opengl ones
    {
        Rect.x *= 2.f / Zen2D->RendererWidth;  Rect.x -= 1;
        Rect.y *= 2.f / Zen2D->RendererHeight; Rect.y -= 1;
        Rect.z *= 2.f / Zen2D->RendererWidth;  
        Rect.w *= 2.f / Zen2D->RendererHeight; 
    }
    
    GLubyte * Data = Zen2D->Rect.Memory + Zen2D->Rect.AllocPos;
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
    Zen2D->Rect.AllocPos += Zen2D->Rect.Size;
    Zen2D->ActiveBatch->DataLength += Zen2D->Rect.Size;
}

internal void
Zen2DPushRect(v4 Rect, v4 Colour) {
    Zen2DPushRectVertices(Rect, Colour, Colour, Colour, Colour);
}

internal void
Zen2DPushBlur(v4 Rect) {
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_BLUR) {
        Zen2D->ActiveBatch = &Zen2D->Batches[Zen2D->BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_BLUR;
        Zen2D->ActiveBatch->Data = Zen2D->Blur.Memory + Zen2D->Blur.AllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // NOTE(Abi): Convert from the screen coordinates to opengl ones
    {
        Rect.x *= 2.f / Zen2D->RendererWidth;  Rect.x -= 1;
        Rect.y *= 2.f / Zen2D->RendererHeight; Rect.y -= 1;
        Rect.z *= 2.f / Zen2D->RendererWidth;  
        Rect.w *= 2.f / Zen2D->RendererHeight; 
    }
    
    GLubyte * Data = Zen2D->Blur.Memory + Zen2D->Blur.AllocPos;
    {
        ((v2 *)Data)[0]  = v2(Rect.x, Rect.y);
        ((v2 *)Data)[1]  = v2(Rect.x + Rect.Width, Rect.y);
        ((v2 *)Data)[2]  = v2(Rect.x, Rect.y + Rect.Height);
        ((v2 *)Data)[3]  = v2(Rect.x + Rect.Width, Rect.y + Rect.Height);
        ((v2 *)Data)[4] = v2(Rect.x + Rect.Width, Rect.y);
        ((v2 *)Data)[5] = v2(Rect.x, Rect.y + Rect.Height);
    }
    Zen2D->Blur.AllocPos += Zen2D->Blur.Size;
    Zen2D->ActiveBatch->DataLength += Zen2D->Blur.Size;
}

internal void
Zen2DPushLineVertices(v2 Start, v2 End, v4 StartColour, v4 EndColour) {
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_LINES) {
        Zen2D->ActiveBatch = &Zen2D->Batches[Zen2D->BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_LINES;
        Zen2D->ActiveBatch->Data = Zen2D->Line.Memory + Zen2D->Line.AllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // NOTE(Abi): Convert to screen coordinates
    {
        Start.x *= 2.f / Zen2D->RendererWidth;  Start.x -= 1;
        Start.y *= 2.f / Zen2D->RendererHeight; Start.y -= 1;
        End.x   *= 2.f / Zen2D->RendererWidth;  End.x   -= 1;
        End.y   *= 2.f / Zen2D->RendererHeight; End.y   -= 1;
    }
    
    GLubyte * Data = Zen2D->Line.Memory + Zen2D->Line.AllocPos;
    {
        ((v2 *)Data)[0]  = v2(Start.x, Start.y);
        ((v2 *)Data)[1]  = v2(StartColour.x, StartColour.y);
        ((v2 *)Data)[2]  = v2(StartColour.z, StartColour.w);
        
        ((v2 *)Data)[3]  = v2(End.x, End.y);
        ((v2 *)Data)[4]  = v2(EndColour.x, EndColour.y);
        ((v2 *)Data)[5]  = v2(EndColour.z, EndColour.w);
    }
    
    Zen2D->Line.AllocPos += Zen2D->Line.Size;
    Zen2D->ActiveBatch->DataLength += Zen2D->Line.Size;
}

internal void
Zen2DPushLine(v2 Start, v2 End, v4 Colour) {
    Zen2DPushLineVertices(Start, End, Colour, Colour);
}

internal void
Zen2DPushTextureRectTint(v4 Destination, texture Texture, v4 Source, v4 Colour) {
    Assert(Texture.ID);
    
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_TEXTURES || Zen2D->ActiveBatch->TexData.ID != Texture.ID) {
        Zen2D->ActiveBatch = &Zen2D->Batches[Zen2D->BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_TEXTURES;
        Zen2D->ActiveBatch->TexData.ID = Texture.ID;
        Zen2D->ActiveBatch->Data = Zen2D->Texture.Memory + Zen2D->Texture.AllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // NOTE(Abi): Convert from the screen coordinates
    {
        Destination.x *= 2.f / Zen2D->RendererWidth;  Destination.x -= 1;
        Destination.y *= 2.f / Zen2D->RendererHeight; Destination.y -= 1;
        Destination.z *= 2.f / Zen2D->RendererWidth;  
        Destination.w *= 2.f / Zen2D->RendererHeight; 
        
        Source.x /= Texture.Width;  Source.Width  /= Texture.Width;
        Source.y /= Texture.Height; Source.Height /= Texture.Height;
    }
    
    GLubyte * Data = Zen2D->Texture.Memory + Zen2D->Texture.AllocPos;
    {
        ((v2 *)Data)[0]  = v2(Destination.x, 
                              Destination.y);
        ((v2 *)Data)[1]  = v2(Colour.x, Colour.y);
        ((v2 *)Data)[2]  = v2(Colour.z, Colour.w);
        ((v2 *)Data)[3]  = v2(Source.x, Source.y);
        
        ((v2 *)Data)[4]  = v2(Destination.x + Destination.Width,
                              Destination.y);
        ((v2 *)Data)[5]  = v2(Colour.x, Colour.y);
        ((v2 *)Data)[6]  = v2(Colour.z, Colour.w);
        ((v2 *)Data)[7]  = v2(Source.x + Source.Width, Source.y);
        
        ((v2 *)Data)[8]  = v2(Destination.x,
                              Destination.y + Destination.Height);
        ((v2 *)Data)[9]  = v2(Colour.x, Colour.y);
        ((v2 *)Data)[10] = v2(Colour.z, Colour.w);
        ((v2 *)Data)[11] = v2(Source.x, Source.y + Source.Height);
        
        ((v2 *)Data)[12] = v2(Destination.x + Destination.Width, 
                              Destination.y + Destination.Height);
        ((v2 *)Data)[13] = v2(Colour.x, Colour.y);
        ((v2 *)Data)[14] = v2(Colour.z, Colour.w);
        ((v2 *)Data)[15] = v2(Source.x + Source.Width, Source.y + Source.Height);
        
        ((v2 *)Data)[16] = v2(Destination.x + Destination.Width,
                              Destination.y);
        ((v2 *)Data)[17] = v2(Colour.x, Colour.y);
        ((v2 *)Data)[18] = v2(Colour.z, Colour.w);
        ((v2 *)Data)[19] = v2(Source.x + Source.Width, Source.y);
        
        ((v2 *)Data)[20] = v2(Destination.x,
                              Destination.y + Destination.Height);
        ((v2 *)Data)[21] = v2(Colour.x, Colour.y);
        ((v2 *)Data)[22] = v2(Colour.z, Colour.w);
        ((v2 *)Data)[23] = v2(Source.x, Source.y + Source.Height);
    }
    Zen2D->Texture.AllocPos += Zen2D->Texture.Size;
    Zen2D->ActiveBatch->DataLength += Zen2D->Texture.Size;
}

internal void
Zen2DPushTextureRect(v4 Destination, texture Texture, v4 Source) {
    v4 Colour = v4(1.f, 1.f, 1.f, 1.f);
    Zen2DPushTextureRectTint(Destination, Texture, Source, Colour);
}


internal void
Zen2DPushTexture(texture Texture, v2 Position) {
    Zen2DPushTextureRect(v4(Position.x, Position.y, Texture.Width, Texture.Height), Texture, v4(0, 0, Texture.Width, Texture.Height));
}

internal v2
Zen2DPushTextFontColourN(const char * String, u32 StringLength, font * Font, v2 StartPosition, f32 FontSize, v4 Colour) {
    Assert(Zen2DIsFontValid(Font));
    
    f32 FontScale = FontSize / (f32)Font->Size;
    if(!Zen2D->ActiveBatch || Zen2D->ActiveBatch->Type != ZEN2D_BATCH_TEXT ||
       Zen2D->ActiveBatch->FontData.p->Texture.ID != Font->Texture.ID) {
        Zen2D->ActiveBatch = &Zen2D->Batches[Zen2D->BatchesCount++];
        Zen2D->ActiveBatch->Type = ZEN2D_BATCH_TEXT;
        Zen2D->ActiveBatch->FontData.p = Font;
        Zen2D->ActiveBatch->Data = Zen2D->Text.Memory + Zen2D->Text.AllocPos;
        Zen2D->ActiveBatch->DataLength = 0;
    }
    
    // TODO(Abi): Center x/y, float right etc
    v2 Cursor = StartPosition;
    
    for(int i = 0; i < StringLength; ++i) {
        if(String[i] == '\n') {
            // HACK(Abi): 0.5 * Font Base is just a made up value
            Cursor.y -= Font->LineHeight * FontScale - 0.5 * Font->Base * FontScale; 
            Cursor.x = StartPosition.x;
            continue;
        }
        
        font_glyph Glyph = Font->Glyphs[String[i] - Font->LowestChar];
        
        v4 Destination = v4(Cursor.x + (Glyph.XOffset * FontScale),
                            Cursor.y + (Font->Base + 1) * FontScale -  ((Glyph.YOffset + Glyph.Height) * FontScale),
                            Glyph.Width * FontScale,
                            Glyph.Height * FontScale);
        
        // HARDCODE(Abi): 
        v4 Source = v4(Glyph.x,
                       Glyph.y + Glyph.Height,
                       Glyph.Width,
                       Glyph.Height);
        
        // NOTE(Abi): Convert to Screen Coordinates
        {
            Destination.x *= 2.f / Zen2D->RendererWidth;  Destination.x -= 1;
            Destination.y *= 2.f / Zen2D->RendererHeight; Destination.y -= 1;
            Destination.z *= 2.f / Zen2D->RendererWidth;  
            Destination.w *= 2.f / Zen2D->RendererHeight; 
            
            Source.x /= Font->Texture.Width;  Source.Width  /= Font->Texture.Width;
            Source.y /= Font->Texture.Height; Source.Height /= Font->Texture.Height;
            Source.y = 1.0 - Source.y;
        }
        
        GLubyte * Data = Zen2D->Text.Memory + Zen2D->Text.AllocPos;
        {
            ((v2 *)Data)[0]  = v2(Destination.x, 
                                  Destination.y);
            ((v2 *)Data)[1]  = v2(Colour.x, Colour.y);
            ((v2 *)Data)[2]  = v2(Colour.z, Colour.w);
            ((v2 *)Data)[3]  = v2(Source.x, Source.y);
            
            ((v2 *)Data)[4]  = v2(Destination.x + Destination.Width,
                                  Destination.y);
            ((v2 *)Data)[5]  = v2(Colour.x, Colour.y);
            ((v2 *)Data)[6]  = v2(Colour.z, Colour.w);
            ((v2 *)Data)[7]  = v2(Source.x + Source.Width, Source.y);
            
            ((v2 *)Data)[8]  = v2(Destination.x,
                                  Destination.y + Destination.Height);
            ((v2 *)Data)[9]  = v2(Colour.x, Colour.y);
            ((v2 *)Data)[10] = v2(Colour.z, Colour.w);
            ((v2 *)Data)[11] = v2(Source.x, Source.y + Source.Height);
            
            ((v2 *)Data)[12] = v2(Destination.x + Destination.Width, 
                                  Destination.y + Destination.Height);
            ((v2 *)Data)[13] = v2(Colour.x, Colour.y);
            ((v2 *)Data)[14] = v2(Colour.z, Colour.w);
            ((v2 *)Data)[15] = v2(Source.x + Source.Width, Source.y + Source.Height);
            
            ((v2 *)Data)[16] = v2(Destination.x + Destination.Width,
                                  Destination.y);
            ((v2 *)Data)[17] = v2(Colour.x, Colour.y);
            ((v2 *)Data)[18] = v2(Colour.z, Colour.w);
            ((v2 *)Data)[19] = v2(Source.x + Source.Width, Source.y);
            
            ((v2 *)Data)[20] = v2(Destination.x,
                                  Destination.y + Destination.Height);
            ((v2 *)Data)[21] = v2(Colour.x, Colour.y);
            ((v2 *)Data)[22] = v2(Colour.z, Colour.w);
            ((v2 *)Data)[23] = v2(Source.x, Source.y + Source.Height);
        }
        
        Zen2D->Text.AllocPos += Zen2D->Text.Size;
        Zen2D->ActiveBatch->DataLength += Zen2D->Texture.Size;
        Cursor.x += Glyph.XAdvance * FontScale;
    }
    
    return Cursor;
}

internal v2
Zen2DPushTextFontN(const char * Text, u32 Length, font * Font, v2 Position, f32 Size) {
    v4 Colour = v4(1.f, 1.f, 1.f, 1.f);
    return Zen2DPushTextFontColourN(Text, Length, Font, Position, Size, Colour);
}

internal v2
Zen2DPushTextN(const char * Text, u32 Length, v2 Position, f32 Size) {
    return Zen2DPushTextFontN(Text, Length, Zen2D->DefaultFont, Position, Size);
}

internal v2
Zen2DPushText(const char * Text, v2 Position, f32 Size) {
    return Zen2DPushTextFontN(Text, StringLength(Text), Zen2D->DefaultFont, Position, Size);
}


internal void
Zen2DPushTextInBox(char * String, f32 Size, v4 Box) {
    
}

internal void
Zen2DBeginFrame() {
    
    {
        OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        OpenGLBindFramebuffer(0);
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // NOTE(Abi): Reset the memory from the previous frame
#define ZEN2DBATCH(name, ...) Zen2D->name.AllocPos = 0;
#include "zen2d_batch_data_types.inc"
    Zen2D->BatchesCount = 0;
    Zen2D->ActiveBatch = 0;
    
    // NOTE(Abi): Transfer Data from platform
    {
        Zen2D->RendererWidth  = Platform->ScreenWidth;
        Zen2D->RendererHeight = Platform->ScreenHeight;
    }
}

internal void
Zen2DEndFrame() {
    // IDEA(Abi): could always have MAIN framebuffer bound, and only reset at the end of the blur/other effects?
    
    for(i32 i = 0; i < Zen2D->BatchesCount; ++i) {
        zen2d_batch * Batch = &Zen2D->Batches[i];
        switch(Batch->Type) {
            case ZEN2D_BATCH_RECTS: {
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_RECTANGLES]);
                glBindVertexArray(Zen2D->Rect.VAO);
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Rect.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                    i32 Count = Batch->DataLength/Zen2D->Rect.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            case ZEN2D_BATCH_LINES: {
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_LINES]);
                glBindVertexArray(Zen2D->Line.VAO);
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Line.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                    i32 Count = Batch->DataLength/Zen2D->Line.Stride;
                    glDrawArrays(GL_LINES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            case ZEN2D_BATCH_TEXTURES: {
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_TEXTURES]);
                glBindVertexArray(Zen2D->Texture.VAO);
                {
                    glBindTexture(GL_TEXTURE_2D, Batch->TexData.ID);
                    glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Texture.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                    i32 Count = Batch->DataLength/Zen2D->Texture.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
            } break;
            
            case ZEN2D_BATCH_TEXT: {
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_TEXT]);
                glBindVertexArray(Zen2D->Text.VAO);
                {
                    glBindTexture(GL_TEXTURE_2D, Batch->FontData.p->Texture.ID);
                    glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Text.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                    i32 Count = Batch->DataLength/Zen2D->Text.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                glBindVertexArray(0);
                
            } break;
            
            case ZEN2D_BATCH_BLUR: {
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_EFFECTS]);
                // TODO(Abi): Maybe only set once and never change?
                glClearColor(0.f, 0.f, 0.f, 0.f); 
                glClear(GL_COLOR_BUFFER_BIT);
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_BLUR]);
                // NOTE(Abi): Don't have to worry about blend func here as blur texture has opacity of 1.0
                glBindVertexArray(Zen2D->Blur.VAO);
                {
                    glBindTexture(GL_TEXTURE_2D, Zen2D->Framebuffer[ZEN2D_FBO_MAIN].Texture);
                    glBindBuffer(GL_ARRAY_BUFFER, Zen2D->Blur.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, Batch->DataLength, Batch->Data);
                    i32 Count = Batch->DataLength/Zen2D->Blur.Stride;
                    glDrawArrays(GL_TRIANGLES, 0, Count);
                }
                
                // NOTE(Abi): Draw back onto the main framebuffer
                OpenGLBindFramebuffer(&Zen2D->Framebuffer[ZEN2D_FBO_MAIN]);
                glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_FBO_BLIT]);
                glBindVertexArray(Zen2D->FramebufferBlit.VAO);
                {
                    glBindTexture(GL_TEXTURE_2D, Zen2D->Framebuffer[ZEN2D_FBO_EFFECTS].Texture);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                glBindVertexArray(0);
            } break;
            
            default: Assert("[Zen2D] Batch had an invalid type" == 0);
        }
    }
    
    // NOTE(Abi) Copy main framebuffer to the default one.
    // ugh
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(Zen2D->Shaders[ZEN2D_SHADER_FBO_BLIT]);
    glBindTexture(GL_TEXTURE_2D, Zen2D->Framebuffer[ZEN2D_FBO_MAIN].Texture);
    glBindVertexArray(Zen2D->FramebufferBlit.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
}