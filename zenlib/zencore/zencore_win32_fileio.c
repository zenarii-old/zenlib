internal FILETIME
Win32GetLastWriteTime(char * Path) {
    FILETIME LastWriteTime = {0};
    
    WIN32_FIND_DATA FindData;
    HANDLE FileHandle = FindFirstFileA(Path, &FindData);
    
    if(FileHandle != INVALID_HANDLE_VALUE) {
        LastWriteTime = FindData.ftLastWriteTime;
        FindClose(FileHandle);
    }
    
    return LastWriteTime;
}

internal char *
Win32LoadFile(const char * Path, b32 Temporary, i32 * BufferLength) {
    // TODO(abi): check if need to change security attribs
    HANDLE FileHandle = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ, NULL,
                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    char * Buffer = 0;
    memory_arena * Arena = Temporary ?
        &GlobalPlatform.ScratchArena : &GlobalPlatform.PermenantArena;
    
    if(FileHandle != INVALID_HANDLE_VALUE) {
        DWORD FileSize = GetFileSize(FileHandle, 0);
        Buffer = MemoryArenaAlloc(Arena, sizeof(char) * (FileSize + 1));
        DWORD BytesRead = 0;
        ReadFile(FileHandle, Buffer, FileSize, &BytesRead, 0);
        Buffer[BytesRead-1] = '\0';
        if(BufferLength) *BufferLength = BytesRead;
    }
    else {
        DWORD Error = GetLastError();
        Log("Failed to load file %s", Path);
    }
    
    CloseHandle(FileHandle);
    return Buffer;
}