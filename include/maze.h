#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "actor.h"
#include "game.h"
#include "items.h"
#include "point.h"
#include "tile.h"

struct maze {
    int rows;
    int columns;
    struct point start;
    struct tile ***grid;
};

struct maze* new_maze(int);
void MazeSetGrid(struct maze*, int);
void delete_maze(struct maze*);
struct maze* init_maze_dimensions(struct maze*, FILE*);
struct maze* init_maze_grid(struct maze*);
void MazeMessage(struct maze* m, const char*);
void MazeMessagePause(struct maze* m, const char*);
void MazePrintMap(struct maze*, struct actor*);
void MazeReadMap(struct maze*, FILE*);
struct maze* MazeSetTile(struct maze*, struct point, enum TileType, int);
struct maze* MazeSetTileEnemy(struct maze*, struct point, struct actor*);
struct maze* MazeSetTileCoins(struct maze* m, struct point p);
struct maze* MazeSetTileItem(struct maze* m, struct point p, struct item* i);
void MazeStats(struct maze* m, struct actor*);
#endif
