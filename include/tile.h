#ifndef TILE
#define TILE

#define PLAYER_CHAR '@'

enum TileType {
    wall,
    floor,
    pit
};

struct tile {
    enum TileType type;
    int character;
};

void delete_tile(struct tile*);
struct tile* new_tile();

#endif
