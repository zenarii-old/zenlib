
internal void
Zen2DInitCommon(void) {
    stbi_set_flip_vertically_on_load(1);
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
        character * Glyph = Font->Glyphs + (String[i] - ' ');
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