#ifndef ACTOR
#define ACTOR

#include "items.h"
#include "point.h"

#define MOBLIN 'm'
#define BIG_MOBLIN 'M'
#define GOBLIN 'g'
#define BIG_GOBLIN 'G'
#define BAT 'w'
#define BIG_BAT 'W'

struct actor {
    int character;
    int hp;
    int attack;
    int damage;
    int defense;
    int coins;
    enum items item;
    struct point p;
};

struct actor *new_actor(int character);
char* actor_name(struct actor*);

#endif
