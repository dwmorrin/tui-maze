#ifndef ENEMY
#define ENEMY

#include "items.h"
#include "point.h"

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
    int coins;
    enum items item;
    struct point p;
};

struct enemy *new_enemy(int character);
char* enemy_name(struct enemy*);

#endif
