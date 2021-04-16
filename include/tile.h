#ifndef TILE
#define TILE

#include "items.h"
#include "actor.h"

#define PLAYER_CHAR '@'

enum TileType {
    false_wall,
    wall,
    floor,
    pit,
    stairs
};

enum tile_what {
    none,
    coins,
    item,
    actor
};

struct tile {
    enum TileType type;
    int character;
    enum tile_what what;
    struct item *item_ref;
    struct actor *actor_ref;
};

void delete_tile(struct tile*);
struct tile* new_tile();
int tile_character(struct tile*);

#endif
