internal void *
Win32HeapAlloc(u32 Size) {
    return HeapAlloc(GetProcessHeap(), 0, Size);
}

internal void
Win32HeapFree(void * Memory) {
    HeapFree(GetProcessHeap(), 0, Memory);
}
