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

internal memory_arena
MemoryArenaInit(unsigned char * Memory, size_t MemorySize) {
    memory_arena Arena = {0};
    Arena.MemorySize = MemorySize;
    Arena.MemoryLeft = MemorySize;
    Arena.Memory = Memory;
    return Arena;
}

internal uintptr_t
MemoryArenaAlignForward(uintptr_t Pointer, size_t Alignment) {
    uintptr_t p, a, Modulo;
    HardAssert(IsPowerOfTwo(Alignment));
    
    p = Pointer;
    a = Alignment;
    
    // NOTE(Abi): This is identical to p % a since a = 2^k
    Modulo = p & (a-1);
    if(Modulo != 0) {
        // NOTE(Abi): p unaligned
        p += a - Modulo;
    }
    
    return p;
}

internal void *
_MemoryArenaAllocAligned(memory_arena * Arena, size_t Size, size_t Alignment) {
    uintptr_t CurrentPointer = (uintptr_t)Arena->Memory + (uintptr_t)Arena->Offset;
    uintptr_t Offset = MemoryArenaAlignForward(CurrentPointer, Alignment);
    Offset -= (uintptr_t)Arena->Memory;
    
    HardAssert(Size <= Arena->MemoryLeft);
    
    Arena->Offset = Offset + Size;
    Arena->MemoryLeft = (Arena->MemorySize) - (Arena->Offset);
    
    void * Pointer = &Arena->Memory[Offset];
    memset(Pointer, 0, Size);
    return Pointer;
}

internal void *
MemoryArenaAlloc(memory_arena * Arena, size_t Size) {
    return _MemoryArenaAllocAligned(Arena, Size, DEFAULT_ALIGNMENT);
}

// NOTE(Abi): This doesn't zero the data, but don't any pointers after this will be garbage!
internal void
MemoryArenaClear(memory_arena * Arena) {
    Arena->Offset = 0;
    Arena->MemoryLeft = Arena->MemorySize;
}