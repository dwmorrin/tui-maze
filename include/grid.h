#ifndef GRID
#define GRID

#include "tile.h"

struct tile ***new_grid();
void delete_grid(struct tile***, int rows, int columns);

#endif
