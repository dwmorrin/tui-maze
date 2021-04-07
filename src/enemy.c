#include <stdlib.h>

#include "enemy.h"
#include "items.h"

struct enemy *new_enemy() {
    struct enemy *e = malloc(sizeof(struct enemy));
    e->hp = 10;
    e->attack = 1;
    e->damage = 0;
    e->defense = 1;
    e->item = food;
    return e;
}
