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

//NOTE(Abi): Defines
#define global static
#define internal static
#define local static

#define Kilobytes(n) (n * 1024)
#define Megabytes(n) (Kilobytes(n) * 1024)
#define Gigabytes(n) (Megabytes(n) * 1024)

#define unused(x) (void)(x)

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

// NOTE(Abi): Zencore headers
#include "zencore_platform.h"
#include "zencore_debug.c"

#endif //ZENCORE_H
