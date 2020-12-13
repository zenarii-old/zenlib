#define MemorySet memset
#define MemoryCopy memcpy

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif

// NOTE(Abi) A fixed sized memory arena.
typedef struct memory_arena memory_arena;
struct memory_arena {
    size_t Offset;
    unsigned char * Memory;
    size_t MemorySize;
    size_t MemoryLeft;
};

internal void   MemoryArenaClear(memory_arena * Arena);
internal void * MemoryArenaAlloc(memory_arena * Arena, size_t Size);
internal char * MemoryArenaAllocStringf(memory_arena * Arena, char * Format, ...);
