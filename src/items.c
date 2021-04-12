#include <stdlib.h>
#include <string.h>

#include "items.h"

char *items_name(enum items it) {
    switch (it) {
        case sword:
            return "sword";
        case food:
            return "food";
        default:
            return "mysterious item";
    }
}
