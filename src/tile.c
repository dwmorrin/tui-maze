#include <stdlib.h>

#include "tile.h"

struct tile* new_tile() {
    struct tile* t = malloc(sizeof(struct tile));
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
