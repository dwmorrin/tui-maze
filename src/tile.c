#include <stdlib.h>

#include "tile.h"

struct tile* new_tile() {
    struct tile* t = malloc(sizeof(struct tile));
    t->type = pit;
    t->character = ' ';
    t->what = none;
    t->item = noitem;
    t->enemy_ref = NULL;
    return t;
}

void delete_tile(struct tile* t) {
    free(t);
}
