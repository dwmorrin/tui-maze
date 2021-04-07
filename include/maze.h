#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "point.h"
#include "tile.h"
#include "enemy.h"
#include "items.h"

enum move {
    left = 'a',
    right = 'd',
    down = 's',
    up = 'w'
};

#define ENEMIES_SIZE 64

#define INVENTORY_SIZE 16

struct maze {
    int rows;
    int columns;
    struct point player;
    int coins;
    int nextEnemy;
    enum items *inventory;
    struct enemy **enemies;
    struct tile ***grid;
};

struct maze* new_maze(const char*);
void delete_maze(struct maze*);
struct maze* init_maze_dimensions(struct maze*, FILE*);
struct maze* init_maze_grid(struct maze*);
void MazeMessage(struct maze* m, const char*);
struct maze* MazePlayerMove(struct maze*, enum move);
void MazePrintMap(struct maze*);
void MazeReadMap(struct maze*, FILE*);
struct maze* MazeSetTile(struct maze*, struct point, enum TileType, int);
struct maze* MazeSetTileWhat(struct maze*, struct point, enum TileType, int, enum Stuff);
struct maze* MazeSetTileEnemy(struct maze*, struct point, enum TileType, struct enemy*);
struct maze* MazeSetPlayer(struct maze*, struct point);
struct maze* MazeMovePlayer(struct maze*, enum move);
int items_token(enum items);
#endif
