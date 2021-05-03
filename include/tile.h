#ifndef TILE
#define TILE

#include <stdbool.h>

#include "actor.h"
#include "items.h"
#include "point.h"
#include "tui.h"

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
    struct point p;
    struct item *item_ref;
    struct actor *actor_ref;
};

void delete_tile(struct tile*);
struct tile* new_tile(int row, int column);
int tile_character(struct tile*);
enum color_pair tile_color(struct tile*);
bool TileIsEmptyFloor(struct tile*);


#endif
