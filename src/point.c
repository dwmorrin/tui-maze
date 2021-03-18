#include <stdlib.h>
#include "point.h"

struct point* new_point(int x, int y) {
    struct point *p = malloc(sizeof(struct point*));
    p->x = x;
    p->y = y;
    return p;
}

void delete_point(struct point* p) {
    free(p);
}
