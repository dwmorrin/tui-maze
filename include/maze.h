#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "point.h"
#include "tile.h"

struct maze {
    struct tile ***grid;
    int rows;
    int columns;
    struct point player;
};

struct maze* new_maze(const char*);
void delete_maze(struct maze*);
struct maze* init_maze_dimensions(struct maze*, FILE*);
struct maze* init_maze_grid(struct maze*);
void MazePrintMap(struct maze*);
void MazeReadMap(struct maze*, FILE*);
struct maze* MazeSetTile(struct maze*, struct point, enum TileType, int);
struct maze* MazeSetPlayer(struct maze*, struct point);

#endif
