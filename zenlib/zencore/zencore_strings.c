#define StringLength strlen
#define StringCopy   strcpy
#define StringCopyN  strncpy

internal char
CharacterToUpper(char c) {
    if('a' <= c && c <= 'z') c += 'A' - 'a';
    return c;
}

internal char
CharacterToLower(char c) {
    if('A' <= c && c <= 'Z') c += 'a' - 'A';
    return c;
}

internal b8
CharacterIsDigit(char c) {
    return ('0' <= c && c <= '9');
}

internal b8
CharacterIsAlpha(char c) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

internal b8
StringCompareCaseSensitiveN(const char * a, const char * b, u32 n) {
    b8 Match = 0;
    
    if(a && b && n) {
        Match = 1;
        for(i32 i = 0; i < n; ++i) {
            if(a[i] != b[i]) {
                Match = 0;
                break;
            }
        }
    }
    
    return Match;
}

internal b8
StringCompareCaseSensitive(const char * a, const char * b) {
    return StringCompareCaseSensitiveN(a, b, StringLength(a));
}

internal b8
StringCompareCaseInsensitiveN(const char * a, const char * b, i32 n) {
    b8 Match = 0;
    
    if(a && b && n) {
        Match = 1;
        for(i32 i = 0; i < n; ++i) {
            if(CharacterToLower(a[i]) != CharacterToLower(b[i])) {
                Match = 0;
                break;
            }
        }
    }
    
    return Match;
}


internal b8
StringCompareCaseInsensitive(const char * a, const char * b) {
    return StringCompareCaseInsensitiveN(a, b, strlen(a));
}

internal i32
GetIntFromString(const char * String) {
    b32 IsNegative = 0;
    if(String && *String == '-') {
        IsNegative = 1;
        String++;
    }
    
    i32 Value = 0;
    for(i32 i = 0; String[i] && CharacterIsDigit(String[i]); ++i) {
        Value *= 10;
        Value += (String[i] - '0');
    }
    if(IsNegative) Value *= -1;
    
    return Value;
}
