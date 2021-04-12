#ifndef MAZE
#define MAZE

#include <stdio.h>

#include "point.h"
#include "tile.h"
#include "actor.h"
#include "items.h"

enum move {
    left = 'a',
    right = 'd',
    down = 's',
    up = 'w'
};

enum action {
    eat = 'e'
};

#define ENEMIES_SIZE 64

#define INVENTORY_SIZE 16

struct maze {
    int rows;
    int columns;
    struct actor *player;
    int coins;
    int nextEnemy;
    enum item_type *inventory;
    struct actor **enemies;
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
struct maze* MazeSetTileWhat(struct maze*, struct point, enum TileType, int, enum tile_what);
struct maze* MazeSetTileEnemy(struct maze*, struct point, enum TileType, struct actor*);
struct maze* MazeSetPlayer(struct maze*, struct point);
int MazeMovePlayer(struct maze*, enum move);
int items_token(enum item_type);
void MazeStats(struct maze* m);
int MazePlayerEat(struct maze *, int);
int MazeBattle(struct maze *, int, int, int);
#endif
