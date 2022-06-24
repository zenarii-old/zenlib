
internal void *
ZenLoadFont(const char * Path, i32 FontHeight, void ** Glyphs) {
    i32 DataLength = 0;
    char * Data = Platform->LoadFile(Path, 0, &DataLength);    
    
    FT_Face Face = {0};
    i32 Error = FT_New_Memory_Face(GlobalFreeType, Data, DataLength, 0, &Face);
    
    if(Error == FT_Err_Unknown_File_Format) {
        LogWarning("Failed to load font %s, unrecognised file format", Path);
    }
    else if(Error) {
        LogWarning("Failed to load font %s", Path);
    }
    
    Error = FT_Set_Pixel_Sizes(Face, 0, FontHeight);
    if (Error) LogWarning("Attempted to set font %s pizel size to an unlisted value", Path);
    
    // NOTE(Abi): Assuming that the max dimensions of each glyph are FontHeight x FontHeight
    // NOTE(Abi): bitmap will be 10 glyphs by 10 glyphs as min(Square) > 95 is 100.
    // TODO(abiab): check font width
    i32 FontBitmapSize = (10 * FontHeight) * (10 * FontHeight) * sizeof(u8);
    u8 * FontBitmap = MemoryArenaAlloc(&Platform->ScratchArena, FontBitmapSize);
    
    character * Characters = Platform->HeapAlloc(sizeof(character) * 95);
    character * CurrentCharacter = 0;
    *Glyphs = Characters;
    
    for(i32 CharacterCode = ' '; CharacterCode < 127; ++CharacterCode) {
        Error = FT_Load_Char(Face, CharacterCode, FT_LOAD_RENDER);
        if(Error) LogWarning("Failed to load character %c in font %s", CharacterCode, Path);
        
        // Note(Abi): Fill in the glyph info
        CurrentCharacter = &Characters[CharacterCode - ' '];
        {
            CurrentCharacter->XOffset  = Face->glyph->bitmap_left;
            CurrentCharacter->YOffset  = Face->glyph->bitmap_top;
            CurrentCharacter->XAdvance = Face->glyph->advance.x / 64;
            CurrentCharacter->Width    = Face->glyph->bitmap.width;
            CurrentCharacter->Height   = Face->glyph->bitmap.rows;
            CurrentCharacter->x        = ((CharacterCode - ' ') % 10) * FontHeight;
            CurrentCharacter->y        = ((CharacterCode - ' ') / 10) * FontHeight;
        }
        
        // NOTE(Abi): Draw to bitmap
        i32 XGlyph = (CharacterCode - ' ') % 10;
        i32 YGlyph = (CharacterCode - ' ') / 10;
        
        i32 XCursor = XGlyph * FontHeight;
        i32 YCursor = YGlyph * FontHeight;
        
        for(i32 x = 0; x < CurrentCharacter->Width; ++x) {
            for(i32 y = 0; y < CurrentCharacter->Height; ++y) {
                i32 XWrite = XCursor + x;
                i32 YWrite = YCursor + y;
                FontBitmap[XWrite + YWrite * (10 * FontHeight)] =
                ((u8*)Face->glyph->bitmap.buffer)[x + y * Face->glyph->bitmap.width];
            }
        }
        
        // move the position of the drawer
    }
    
    FT_Done_Face(Face);
    
    return FontBitmap;
}
