#ifndef ENEMY
#define ENEMY

#include "items.h"

#define MOBLIN 'm'
#define BIG_MOBLIN 'M'
#define GOBLIN 'g'
#define BIG_GOBLIN 'G'
#define BAT 'w'
#define BIG_BAT 'W'

struct enemy {
    int character;
    int hp;
    int attack;
    int damage;
    int defense;
    enum items item;
};

struct enemy *new_enemy(int character);

#endif
