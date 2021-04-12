#include <stdlib.h>
#include <string.h>

#include "items.h"

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
