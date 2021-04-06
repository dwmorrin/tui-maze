#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "point.h"
#include "tile.h"

enum move {
    left = 'a',
    right = 'd',
    down = 's',
    up = 'w'
};

#define INVENTORY_SIZE 16
enum items {
    noitem,
    sword,
    shield,
    armor,
    food
};

struct maze {
    int rows;
    int columns;
    struct point player;
    int coins;
    enum items *inventory;
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
struct maze* MazeSetPlayer(struct maze*, struct point);
struct maze* MazeMovePlayer(struct maze*, enum move);
int items_token(enum items);
#endif
