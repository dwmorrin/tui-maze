#ifndef ENEMY
#define ENEMY

#include "items.h"

struct enemy {
    int character;
    int hp;
    int attack;
    int damage;
    int defense;
    enum items item;
};

struct enemy *new_enemy();

#endif
