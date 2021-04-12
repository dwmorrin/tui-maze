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

char *item_name(enum item_type it) {
    switch (it) {
        case sword:
            return "sword";
        case food:
            return "food";
        default:
            return "mysterious item";
    }
}
