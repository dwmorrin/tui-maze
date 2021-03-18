#ifndef POINT
#define POINT

struct point {
    int x, y;
};

struct point* new_point(int x, int y);
void delete_point(struct point*);

#endif
