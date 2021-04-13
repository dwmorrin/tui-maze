#include <stdlib.h>
#include <string.h>

#include "actor.h"
#include "items.h"

struct actor *new_actor(int character) {
    struct actor *a = malloc(sizeof(struct actor));
    a->character = character;
    a->hp = 10;
    a->attack = 1;
    a->damage = 0;
    a->defense = 1;
    a->coins = 0;
    a->weapon = new_item(sword, 1, '/');
    a->food = new_item(food, 2, '*');
    a->p.x = 0;
    a->p.y = 0;
    return a;
}

void delete_actor(struct actor *a) {
    if (a->weapon) delete_item(a->weapon);
    if (a->food) delete_item(a->food);
    free(a);
}

char *actor_name(struct actor *a) {
    switch (a->character) {
        case BIG_MOBLIN:
            return "big moblin";
        case MOBLIN:
            return "moblin";
        default:
            return "mysterious figure";
    }
}
