#ifndef TILE
#define TILE

#include "items.h"

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
    enum items item;
    struct enemy *enemy_ref;
};

void delete_tile(struct tile*);
struct tile* new_tile();

#endif
