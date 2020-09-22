#ifndef ZEN2D_H
#define ZEN2D_H

// NOTE(Abi): defined in the relevant zen2d_<renderer>.h
typedef struct zen2d zen2d;
typedef struct texture texture;

#define ZEN2D_MAX_BATCHES 1024
typedef enum zen2d_batch_type zen2d_batch_type;
enum zen2d_batch_type {
    ZEN2D_BATCH_NULL,
    ZEN2D_BATCH_RECTS,
    ZEN2D_BATCH_LINES,
    
    ZEN2D_BATCH_COUNT
};

typedef struct zen2d_batch zen2d_batch;
struct zen2d_batch {
    zen2d_batch_type Type;
    void * Data;
    u32 DataLength;
};

global zen2d * Zen2D;

#define ZEN2D_COMMON \
struct { \
zen2d_batch * ActiveBatch; \
zen2d_batch Batches[ZEN2D_MAX_BATCHES]; \
u32 BatchesCount; \
\
f32 RendererWidth; \
f32 RendererHeight; \
} 

//TODO(Zen): Aim is to get a system where I can push rectangles and borders of different amounts and make sure that the visuals work

internal void Zen2DPushRect(v4 Rect, v4 Colour);
internal void Zen2DPushRectVertices(v4 Rect, v4, v4, v4, v4);

internal void Zen2DPushLineVertices(v2 Start, v2 End, v4 StartColour, v4 EndColour);
internal void Zen2DPushLine(v2 Start, v2 End, v4 Colour);

internal void Zen2DInit(memory_arena * Arena);
internal void Zen2DBeginFrame(void);
internal void Zen2DEndFrame(void);



#ifdef USE_OPENGL
#include "zen2d_opengl.h"
#endif

#endif //ZEN2D_H
