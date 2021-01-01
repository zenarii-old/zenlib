
internal void
Zen2DInitCommon(void) {
    stbi_set_flip_vertically_on_load(1);
}

internal font
Zen2DLoadFontFromFNTAndPNG(const char * FNTPath, const char * PNGPath) {
    char * FNTData = Platform->LoadFile(FNTPath, 1);
    i32 Width, Height, Channels;
    void * PNGData = stbi_load(PNGPath, &Width, &Height, &Channels, 0);
    
    font_glyph Glyphs[256] = {0};
    font_glyph * CurrentGlyph = 0;
    u32 GlyphCount = 0;
    u32 LowestCharValue = 1000;
    u32 FontSize = 0;
    u32 LineHeight = 0;
    u32 Base = 0;
    v4 Padding = {0};
    
    char Name[64] = {0};
    b32 Commented = 0;
    
    enum font_read_type {
        READ_NONE,
        READ_VALUE,
        READ_NAME
    };
    enum font_read_type ReadType = READ_NONE;
    
    i32 LineNum = 0;
    
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
                        CurrentGlyph = 0;
                        LineNum++;
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
                    else if(StringCompareCaseInsensitive("padding", Name)) {
                        Padding.x = Value;
                        while(FNTData[i] != ',') {++i;} ++i;
                        Value = GetIntFromString(FNTData + i);
                        Padding.y = Value;
                        while(FNTData[i] != ',') {++i;} ++i;
                        Value = GetIntFromString(FNTData + i);
                        Padding.z = Value;
                        while(FNTData[i] != ',') {++i;} ++i;
                        Value = GetIntFromString(FNTData + i);
                        Padding.w = Value;
                    }
                    else if(StringCompareCaseInsensitive("id", Name) && Value != 0) {
                        GlyphCount++;
                        CurrentGlyph = &Glyphs[Value];
                        if(Value < LowestCharValue) LowestCharValue = Value;
                    }
                    else if(StringCompareCaseInsensitive("xadvance", Name)) {
                        // NOTE(Abi): If XAdvance ever gets fucky, check this.
                        CurrentGlyph->XAdvance = Value - (Padding.x + Padding.z);
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
    
    font Font = Zen2DLoadFont(PNGData, Width, Height, Channels, Glyphs + LowestCharValue, 
                              GlyphCount, LineHeight, FontSize, Base, LowestCharValue);
    
    stbi_image_free(PNGData);
    return Font;
}

internal void
Zen2DSetDefaultFont(font * Font) {
    Zen2D->DefaultFont = Font;
}

internal font *
Zen2DGetDefaultFont(void) {
    return Zen2D->DefaultFont;
}

internal f32
Zen2DGetStringWidthN(const char * String, font * Font, f32 Size, u32 n) {
    f32 MaxLineWidth = 0;
    f32 LineWidth = 0;
    f32 FontScale = Size / Font->Size;
    for(i32 i = 0; i < n; ++i) {
        font_glyph * Glyph = Font->Glyphs + (String[i] - Font->LowestChar);
        if(String[i] == '\n') {
            if(MaxLineWidth < LineWidth) MaxLineWidth = LineWidth;
            LineWidth = 0;
            continue;
        }
        LineWidth += Glyph->XAdvance * FontScale;
    }
    if(MaxLineWidth < LineWidth) MaxLineWidth = LineWidth;
    
    return MaxLineWidth;
}

internal f32
Zen2DGetStringWidth(const char * String, font * Font, f32 Size) {
    return Zen2DGetStringWidthN(String, Font, Size, StringLength(String));
}

#ifdef USE_OPENGL
#include "zen2d_opengl.c"
#endif