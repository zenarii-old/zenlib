#ifndef ZEN2D_H
#define ZEN2D_H
// NOTE(Abi): defined in the relevant zen2d_<renderer>.h
typedef struct zen2d      zen2d;
typedef struct texture    texture;
typedef struct font       font;
typedef struct font_glyph font_glyph;

struct font_glyph {
    i16 x;
    i16 y;
    i16 Width;
    i16 Height;
    i16 XOffset;
    i16 YOffset;
    i16 XAdvance;
};

global zen2d * Zen2D;

#define ZEN2D_COMMON \
struct { \
zen2d_batch * ActiveBatch; \
zen2d_batch Batches[ZEN2D_MAX_BATCHES]; \
u32 BatchesCount; \
\
font DefaultFont_; \
font * DefaultFont;\
\
f32 RendererWidth; \
f32 RendererHeight; \
} 

//
// Drawing
//

internal void Zen2DPushRect(v4 Rect, v4 Colour);
internal void Zen2DPushRectVertices(v4 Rect, v4, v4, v4, v4);

internal void Zen2DPushLineVertices(v2 Start, v2 End, v4 StartColour, v4 EndColour);
internal void Zen2DPushLine(v2 Start, v2 End, v4 Colour);

internal void Zen2DPushTexture(texture Texture, v2 Position);
internal void Zen2DPushTextureRect(v4 Destination, texture Texture, v4 Source);

internal void Zen2DPushBlur(v4 Rect);

//
// Loading
//

internal font Zen2DLoadFontFromFNTAndPNG(const char * FNTPath, const char * PNGPath);
internal font Zen2DLoadFont(void * PNGData, i32 Width, i32 Height, i32 Channels, font_glyph * Glyphs, u32 GlyphCount, u32 LineHeight, u32 FontSize, u32 Base, u32 LowestChar);

internal void Zen2DInitCommon(void);
internal void Zen2DInit(memory_arena * Arena);
internal void Zen2DBeginFrame(void);
internal void Zen2DEndFrame(void);

#ifdef USE_OPENGL
#include "zen2d_opengl.h"
#endif

#endif //ZEN2D_H
