#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ASSERT(x) Assert(x)
// TODO(Abi): Add scratch arena stuff so no allocations
#include "ext/stb_image.h"

internal void
Zen2DInitCommon(void) {
    stbi_set_flip_vertically_on_load(1);
}

internal texture
Zen2DLoadTextureFromPNG(const char * Path) {
    i32 Width, Height, Channels;
    unsigned char * Data = stbi_load(Path, &Width, &Height, &Channels, 0);
    
    texture T = Zen2DLoadTexture(Data, Width, Height, Channels);
    
    stbi_image_free(Data);
    return T;
}

internal font
Zen2DLoadFontFromFNTAndPNG(const char * FNTPath, const char * PNGPath) {
    char * FNTData = Platform->LoadFile(FNTPath);
    i32 Width, Height, Channels;
    void * PNGData = stbi_load(PNGPath, &Width, &Height, &Channels, 0);
    
    font_glyph Glyphs[256] = {0};
    font_glyph * CurrentGlyph = 0;
    u32 GlyphCount = 0;
    u32 LowestCharValue = 1000;
    u32 FontSize = 0;
    u32 LineHeight = 0;
    u32 Base = 0;
    char Name[64] = {0};
    b32 Commented = 0;
    
    enum font_read_type {
        READ_NONE,
        READ_VALUE,
        READ_NAME
    };
    enum font_read_type ReadType = READ_NONE;
    
    for(i32 i = 0; FNTData[i];) {
        if(FNTData[i] == '#') {
            Commented = 1;
        }
        
        if(Commented) {
            if(FNTData[i] == '\n') {
                Commented = 0;
            }
            ++i;
        }
        else {
            switch (ReadType) {
                case READ_NONE: {
                    if(FNTData[i] == '\n') {
#if 0
                        if(CurrentGlyph) {
                            Log("Glyph (%c): %d, %d, %d, %d, %d, %d, %d", (char)(CurrentGlyph - Glyphs), CurrentGlyph->x, CurrentGlyph->y, CurrentGlyph->Width, CurrentGlyph->Height, CurrentGlyph->XOffset, CurrentGlyph->YOffset, CurrentGlyph->XAdvance);
                        }
#endif
                        CurrentGlyph = 0;
                    }
                    
                    if(CharacterIsDigit(FNTData[i]) || FNTData[i] == '-') {
                        ReadType = READ_VALUE;
                    }
                    else if(CharacterIsAlpha(FNTData[i])) {
                        ReadType = READ_NAME;
                    }
                    else {
                        i++;
                    }
                } break;
                
                case READ_NAME: {
                    char * Start = FNTData + i;
                    u32 Length = 0;
                    while(CharacterIsAlpha(FNTData[i])) {
                        ++i; 
                        ++Length;
                    }
                    StringCopyN(Name, Start, Length);
                    ReadType = 0;
                } break;
                
                case READ_VALUE: {
                    // NOTE(Abi): Get the value
                    i32 Value = GetIntFromString(FNTData + i);
                    
                    if(StringCompareCaseInsensitive("chars", Name)) {
                        GlyphCount = Value;
                    }
                    else if(StringCompareCaseInsensitive("id", Name) && Value != 0) {
                        GlyphCount++;
                        CurrentGlyph = &Glyphs[Value];
                        CurrentGlyph->ID = Value;
                        if(Value < LowestCharValue) LowestCharValue = Value;
                    }
                    else if(StringCompareCaseInsensitive("xadvance", Name)) {
                        CurrentGlyph->XAdvance = Value;
                    }
                    else if(StringCompareCaseInsensitive("xoffset", Name)) {
                        CurrentGlyph->XOffset = Value;
                    }
                    else if(StringCompareCaseInsensitive("x", Name)) {
                        CurrentGlyph->x = Value;
                    }
                    else if(StringCompareCaseInsensitive("yoffset", Name)) {
                        CurrentGlyph->YOffset = Value;
                    }
                    else if(StringCompareCaseInsensitive("y", Name)) {
                        CurrentGlyph->y = Value;
                    }
                    else if(StringCompareCaseInsensitive("width", Name)) {
                        CurrentGlyph->Width = Value;
                    }
                    else if(StringCompareCaseInsensitive("height", Name)) {
                        CurrentGlyph->Height = Value;
                    }
                    else if(StringCompareCaseInsensitive("size", Name)) {
                        FontSize = Value;
                    }
                    else if(StringCompareCaseInsensitive("lineHeight", Name)) {
                        LineHeight = Value;
                    }
                    else if(StringCompareCaseInsensitive("base", Name)) {
                        Base = Value;
                    }
                    
                    ReadType = READ_NONE;
                    Name[0] = '\0';
                    i++;
                    
                } break;
            }
            
        }
        
    }
    Log("Lowest char: %c (%d)", LowestCharValue, LowestCharValue);
    Log("Lineheight: %d", LineHeight);
    font Font = Zen2DLoadFont(PNGData, Width, Height, Channels, Glyphs + LowestCharValue, 
                              GlyphCount, LineHeight, FontSize, Base, LowestCharValue);
    
    stbi_image_free(PNGData);
    return Font;
}

#ifdef USE_OPENGL
#include "zen2d_opengl.c"
#endif