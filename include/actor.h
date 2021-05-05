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
    struct item *weapon;
    struct item *food;
    struct point p;
};

struct actor *new_actor(int character);
void delete_actor(struct actor*);
char* actor_name(struct actor*);
int actor_health(struct actor*);

#endif
