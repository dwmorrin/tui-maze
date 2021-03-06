#include <stdlib.h>

#include "tile.h"
#include "tui.h"

struct tile* new_tile(int row, int column) {
    struct tile* t = malloc(sizeof(struct tile));
    t->p.x = column;
    t->p.y = row;
    t->type = pit;
    t->character = ' ';
    t->what = none;
    t->item_ref = NULL;
    t->actor_ref = NULL;
    return t;
}

void delete_tile(struct tile* t) {
    if (t->item_ref) free(t->item_ref);
    if (t->actor_ref) free(t->actor_ref);
    free(t);
}

int tile_character(struct tile *t) {
    switch (t->what) {
        case none:
            return t->character;
        case coins:
            return '$';
        case item:
            return t->item_ref->character;
        case actor:
            return t->actor_ref->character;
    }
}

enum color_pair tile_color(struct tile *t) {
    switch (t->what) {
        case none:
            return white;
        case coins:
            return yellow;
        case item:
            return red;
        case actor:
            return blue;
    }
}

bool TileIsEmptyFloor(struct tile* t) {
    return t->type == floor && t->what == none;
}
