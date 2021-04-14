#include <stdlib.h>
#include <string.h>

#include "items.h"

struct item *new_item(
        enum item_type t,
        int v,
        int c
) {
    struct item *i = malloc(sizeof(struct item));
    i->type = t;
    i->value = v;
    i->character = c;
    return i;
}

void delete_item(struct item *i) { free(i); }

char *item_name(struct item *it) {
    switch (it->type) {
        case sword:
            return "sword";
        case food:
            return "food";
        default:
            return "mysterious item";
    }
}

void copy_item(struct item *src, struct item *dst) {
    dst->type = src->type;
    dst->value = src->value;
    dst->character = src->character;
}

void clear_item(struct item *i) {
    i->type = noitem;
    i->value = 0;
    i->character = ' ';
}
