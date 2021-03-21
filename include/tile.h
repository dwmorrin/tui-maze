#ifndef TILE
#define TILE

#define PLAYER_CHAR '@'

enum TileType {
    false_wall,
    wall,
    floor,
    pit
};

enum Stuff {
    none,
    coins,
    item,
    enemy
};

struct tile {
    enum TileType type;
    int character;
    enum Stuff what;
};

void delete_tile(struct tile*);
struct tile* new_tile();

#endif
