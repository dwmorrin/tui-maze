#ifndef ITEMS
#define ITEMS

enum item_type {
    noitem,
    sword,
    shield,
    armor,
    food
};

struct item {
    enum item_type type;
    int value; // meaning varies by type
    int character; // printable character
};

struct item *new_item(enum item_type t, int v, int c);
void delete_item(struct item*);
char *item_name(struct item*);
void copy_item(struct item *src, struct item *dst);
void clear_item(struct item*);

#endif

