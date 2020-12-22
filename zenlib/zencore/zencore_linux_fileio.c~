internal char * 
LinuxLoadFile(const char * Path, b32 Temporary) {
    FILE * File = fopen(Path, "r");
    if(File == 0) return 0;
    
    fseek(File, 0L, SEEK_END);
    long BytesRead = ftell(File);
    
    fseek(File, 0, SEEK_SET);
    
    memory_arena * Arena = Temporary ? 
        &GlobalPlatform.ScratchArena : &GlobalPlatform.PermenantArena;
    
    char * Buffer = MemoryArenaAlloc(Arena, sizeof(char) * BytesRead);
    if(Buffer == 0) return 0;
    
    fread(Buffer, sizeof(char), BytesRead, File);
    fclose(File);
    
    Buffer[BytesRead] = '\0';
    
    return Buffer;
}