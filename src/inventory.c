#include <stdlib.h>

#include "fatal.h"
#include "inventory.h"
#include "items.h"

struct item **new_inventory() {
    struct item **inv = malloc(sizeof(struct item*) * INVENTORY_SIZE);
    if (!inv) fatal("no memory for a new inventory");
    for (int i = 0; i < INVENTORY_SIZE; ++i)
        inv[i] = new_item(noitem, 0, ' ');
    return inv;
}

void delete_inventory(struct item **inv) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        delete_item(inv[i]);
    }
    free(inv);
}

