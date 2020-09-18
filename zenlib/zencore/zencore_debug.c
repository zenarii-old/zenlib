// TODO(Abi): Clean up file into header etc.
void // TODO(Abi): Use variable args
DebugLog(i32 Type, const char * File, i32 Line, const char * Message) {
    // TEMP(Abi): 
    fprintf(stderr, "(%s:%d) %s\n", File, Line, Message);
}

#define Log(Message) DebugLog(0, __FILE__, __LINE__, Message);