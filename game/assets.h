
#define TILEMAP_WIDTH 14
#define TILEMAP_HEIGHT 10
#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_SCALE 10

typedef enum tile tile;
enum tile {
    BUILDING_CORNER_TL = 0,
    BUILDING_WALL,
    BUILDING_DOOR_BACK,
    BUILDING_CORNER_TR,
    
    BUILDING_WALL_LEFT = TILEMAP_WIDTH * 1,
    BLANK_SPACE,
    WEIRD_CROSS_THING, // TODO(Abi): better name lmao
    BUILDING_WALL_RIGHT,
    
    BUILDING_CORNER_BL = TILEMAP_WIDTH * 2,
    BUILDING_FRONT_WALL,
    BUILDING_FRONT_DOOR,
    BUILDING_CORNER_BR,
};

typedef struct board_object board_object;
typedef void (*bob_callback)(board_object * Bob);

#define BOB_SELECTABLE (1<<0)
#define BOB_BOBBING    (1<<1)
#define BOB_SHRINKING  (1<<2)

struct board_object {
    u32 Flags;
    i32 x, y;
    tile Tile;
    board_object * Next;
    
    // NOTE(Abi): Selectable
    bob_callback LeftClickCallback;
    bob_callback RightClickCallback;
    
    //NOTE(Abi):
    i32 MaxHealth;
    i32 Health;
};

typedef struct map_tile map_tile;
struct map_tile {
    tile Tile;
    u32 Cost;
};

typedef struct map map;
struct map {
    char * Name;
    i32 Width, Height;
    tile * Tiles;
    board_object * Objects;
    board_object * LastObject;
};
