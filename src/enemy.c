#include <stdlib.h>

#include "enemy.h"
#include "items.h"

struct enemy *new_enemy(int character) {
    struct enemy *e = malloc(sizeof(struct enemy));
    e->character = character;
    e->hp = 10;
    e->attack = 1;
    e->damage = 0;
    e->defense = 1;
    e->coins = 0;
    e->item = food;
    e->p.x = 0;
    e->p.y = 0;
    return e;
}
