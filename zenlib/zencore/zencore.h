#ifndef ZENCORE_H
#define ZENCORE_H

#define ZENCORE 1

// TEMP(Abi) Move to debug
#define ZSoftAssert(...)
// TEMP(Abi) Move to memory
#define MemoryCopy memcpy
#define MemorySet memset

// NOTE(Abi): C Standard Library
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

//NOTE(Abi): Defines
#define global static
#define internal static
#define local static

#define Kilobytes(n) (n * 1024)
#define Megabytes(n) (Kilobytes(n) * 1024)
#define Gigabytes(n) (Megabytes(n) * 1024)

#define _Stringify(x) #x
#define Stringify(x) _Stringify(x)

#define unused(x) (void)(x)

#define IsPowerOfTwo(x) !((x) & (x-1))
#define Abs(x) ((x) >= 0 ? (x) : -(x))

#define PI 3.1415926535897f

//NOTE(Abi): Typedefs
typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef float     float32;
typedef double    float64;
typedef int8      bool8;
typedef int16     bool16;
typedef int32     bool32;
typedef int64     bool64;

typedef int8      i8;
typedef int16     i16;
typedef int32     i32;
typedef int64     i64;
typedef uint8     u8;
typedef uint16    u16;
typedef uint32    u32;
typedef uint64    u64;
typedef float32   f32;
typedef float64   f64;
typedef bool8     b8;
typedef bool16    b16;
typedef bool32    b32;
typedef bool64    b64;

typedef struct v2 v2;
struct v2 { f32 x; f32 y; };
#define v2(x, y) (v2){x, y}

typedef struct v3 v3;
struct v3 { f32 x; f32 y; f32 z; };
#define v3(x, y, z) (v3){x, y, z}

typedef struct v4 v4;
struct v4 { 
    f32 x; f32 y; 
    union { 
        struct { f32 z; f32 w; }; 
        struct { f32 Width; f32 Height; }; 
    };
};
#define v4(x, y, z, w) (v4){x, y, {{z, w}}}

typedef struct v3i v3i;
struct v3i { i32 x, y, z; };
#define v3i(x, y, z) (v3i){x, y, z}

#define ExpandRect(rect, n) v4((rect).x + (n), (rect).y + (n), \
(rect).Width - 2 * (n), (rect).Height - 2 * (n))

// NOTE(Abi): Zencore headers
#include "zencore_debug.h"
#include "zencore_memory.h"
#include "zencore_platform.h"

#include "zencore_debug.c"
#include "zencore_memory.c"
#include "zencore_maths.c"
#include "zencore_strings.c"

#ifdef USE_OPENGL
#include "zencore_opengl.c"
#endif

#include "program_options.inc"

#endif //ZENCORE_H
