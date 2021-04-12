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
    a->item = food;
    a->p.x = 0;
    a->p.y = 0;
    return a;
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
