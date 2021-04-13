#ifndef INVENTORY
#define INVENTORY

#include "items.h"

#define INVENTORY_SIZE 16

struct item **new_inventory();
void delete_inventory(struct item **);

#endif
