// TODO(Abi): Clean up file into header etc.
internal void
DebugLog(i32 Type, const char * File, i32 Line, const char * Message, ...) {
    va_list Args;
    va_start(Args, Message);
    fprintf(stderr, "(%s:%d) ", File, Line);
    vfprintf(stderr, Message, Args);
    fprintf(stderr, "\n");
    
    va_end(Args);
    // TODO(Abi): Will need to use OutputStringA or smt on windows
}

internal void
FailedAssert(const char * File, i32 Line, const char * Expression, b32 Crash) {
    fprintf(stderr, "Assert Failed (%s:%d): %s\n", File, Line, Expression);
    
    i32 TitleLength = printf("Assert Failed (%s:%d)", File, Line);
    char * Title = MemoryArenaAlloc(&Platform->ScratchArena, TitleLength + 1);
    sprintf(Title, "Assert Failed (%s:%d)", File, Line);
    
    Platform->Error(Title, Expression);
    
    if(Crash) {
        *(int *)0 = 0;
    }
}