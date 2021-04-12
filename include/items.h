#ifndef ITEMS
#define ITEMS

enum item_type {
    noitem,
    sword,
    shield,
    armor,
    food
};

char *item_name(enum item_type);

#endif

