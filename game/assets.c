internal void
LoadAllAssets(void) {
    GameData->Tilemap = Zen2DLoadTextureFromPNG("Tilemap.png", ZEN2D_TEXTURE_NEAREST);
}

internal u32
TileEnumToTile(tile Tile) {
    u32 TileY = (TILEMAP_HEIGHT-1) - Tile / TILEMAP_WIDTH;
    u32 TileX = Tile % TILEMAP_WIDTH;
    u32 Result = TileY * TILEMAP_WIDTH + TileX;
    Assert(Result < TILEMAP_HEIGHT * TILEMAP_WIDTH);
    return Result;
}

internal void
AddBobToMap(map * Map, board_object * Bob) {
    if(!Map->Objects) {
        Map->Objects = Bob;
    }
    else {
        Map->Objects->Next = Bob;
    }
    Map->LastObject = Bob;
}

internal void
DrawMap(map Map) {
    Zen2DPushText(Map.Name, v2(5, Platform->ScreenHeight - 25), 20);
    for(i32 y = 0; y < Map.Height; ++y) {
        for(i32 x = 0; x < Map.Width; ++x) {
            u32 Tile = Map.Tiles[(y * Map.Width) + x];
            Tile = TileEnumToTile(Tile);
            
            v4 Source = v4((Tile % TILEMAP_WIDTH) * TILE_WIDTH, 
                           (Tile / TILEMAP_WIDTH) * TILE_HEIGHT,
                           TILE_WIDTH, TILE_HEIGHT);
            v4 Destination = v4(x * TILE_WIDTH * TILE_SCALE, 
                                (Map.Height - y - 1) * TILE_HEIGHT * TILE_SCALE, TILE_WIDTH * TILE_SCALE, TILE_HEIGHT * TILE_SCALE);
            
            Destination.x += 0.5 * (Platform->ScreenWidth 
                                    - (TILE_WIDTH * TILE_SCALE * Map.Width));
            Destination.y += 0.5 * (Platform->ScreenHeight
                                    - (TILE_HEIGHT * TILE_SCALE * Map.Height));
            Zen2DPushTextureRect(Destination, GameData->Tilemap, Source);
        }
    }
}

internal void
DoBobs(map Map) {
    i32 MouseX = Platform->MousePosition.x - Platform->ScreenWidth * 0.5f + (TILE_WIDTH * TILE_SCALE * Map.Width) * 0.5f;
    MouseX /= (TILE_WIDTH * TILE_SCALE);
    i32 MouseY = Platform->MousePosition.y - Platform->ScreenHeight * 0.5f + (TILE_HEIGHT * TILE_SCALE * Map.Height) * 0.5f;
    MouseY /= (TILE_HEIGHT * TILE_SCALE); MouseY = Map.Height - MouseY - 1;
    b32 MouseUsed = 0;
    
    local char Buf[32] = {0};
    sprintf(Buf, "(%d, %d)", MouseX, MouseY);
    Zen2DPushText(Buf, v2(5, 5), 20);
    
    for(board_object * Bob = Map.Objects; Bob; Bob = Bob->Next) {
        if((Bob->Flags & BOB_SELECTABLE) && MouseX == Bob->x && MouseY == Bob->y) {
            if(ZenMouseJustDown(0) && Bob->LeftClickCallback) 
                Bob->LeftClickCallback(Bob);
            if(ZenMouseJustDown(2) && Bob->RightClickCallback)
                Bob->RightClickCallback(Bob);
            MouseUsed = 1;
        }
        
    }
    
    if(SelectedUnit) {
        // TODO(Abi): Draw arrow
        //= PathFind(Map, MouseX, Mouse.Y, 
        
        if(ZenMouseJustDown(0) && !MouseUsed) {
            if(0 <= MouseX && MouseX < Map.Width && 0 <= MouseY && MouseY < Map.Height) {
                SelectedUnit->x = MouseX;
                SelectedUnit->y = MouseY;
                SelectedUnit->Flags ^= BOB_SHRINKING;
                SelectedUnit = 0;
                Log("Click");
            }
        }
    }
}

internal void
DrawBobs(map Map) {
    for(board_object * Bob = Map.Objects; Bob; Bob = Bob->Next) {
        u32 Tile = TileEnumToTile(Bob->Tile);
        v4 Source = v4((Tile % TILEMAP_WIDTH) * TILE_WIDTH, 
                       (Tile / TILEMAP_WIDTH) * TILE_HEIGHT,
                       TILE_WIDTH, TILE_HEIGHT);
        v4 Destination = v4(Bob->x * TILE_WIDTH * TILE_SCALE, 
                            (Map.Height - Bob->y - 1) * TILE_HEIGHT * TILE_SCALE, TILE_WIDTH * TILE_SCALE, TILE_HEIGHT * TILE_SCALE);
        
        Destination.x += 0.5 * (Platform->ScreenWidth 
                                - (TILE_WIDTH * TILE_SCALE * Map.Width));
        Destination.y += (int)(0.5 * (Platform->ScreenHeight
                                      - (TILE_HEIGHT * TILE_SCALE * Map.Height)));
        // TODO(Abi): For both of these, make sure shrinking/bobbing doesn't jerk at start
        if(Bob->Flags & BOB_BOBBING)  {
            Destination.y += 2 + (f32)TILE_SCALE * sin(Platform->TotalTime * 3.f);
        }
        if(Bob->Flags & BOB_SHRINKING) {
            Destination =
                ExpandRect(Destination, 0.8 * TILE_SCALE * 
                           sinf(Platform->TotalTime*0.8f) * sinf(Platform->TotalTime*0.8f));
        }
        
        Zen2DPushTextureRect(Destination, GameData->Tilemap, Source);
    }
    
    if(SelectedUnit) {
        f32 StartPos = Platform->ScreenWidth
            - (SelectedUnit->MaxHealth/2) * TILE_WIDTH * TILE_SCALE;
        for(i32 i = 0; i < SelectedUnit->MaxHealth/2; ++i) {
            i32 Offset = 0;
            
            if(i < (SelectedUnit->Health)/2) Offset = 2;
            else if(i < (SelectedUnit->Health+1)/2) Offset = 1;
            
            u32 Tile = 3 * TILEMAP_WIDTH + 4 + Offset;
            v4 Source = v4((Tile % TILEMAP_WIDTH) * TILE_WIDTH, 
                           (Tile / TILEMAP_WIDTH) * TILE_HEIGHT,
                           TILE_WIDTH, TILE_HEIGHT);
            v4 Destination = v4(//Platform->ScreenWidth - (i + 1) * TILE_WIDTH * TILE_SCALE,
                                StartPos + i * TILE_WIDTH * TILE_SCALE,
                                Platform->ScreenHeight - TILE_HEIGHT * TILE_SCALE, TILE_WIDTH * TILE_SCALE, TILE_HEIGHT * TILE_SCALE);
            Zen2DPushTextureRect(Destination, GameData->Tilemap, Source);
        }
    }
    //else if(HoveredUnit) { ... }
}