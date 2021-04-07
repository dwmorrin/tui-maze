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
    e->item = food;
    return e;
}
