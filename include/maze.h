#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "point.h"

struct maze {
    int **grid;
    int rows;
    int columns;
};

struct maze* new_maze(const char*);
void delete_maze(struct maze*);
struct maze* init_maze_dimensions(struct maze*, FILE*);
struct maze* init_maze_grid(struct maze*);
void MazeReadMap(struct maze*, FILE*);
struct maze* MazeSetPoint(struct maze*, struct point, int);

#endif
