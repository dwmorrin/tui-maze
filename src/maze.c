#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"
#include "maze.h"
#include "point.h"
#include "tile.h"
#include "tui.h"

struct maze* new_maze(const char* filename) {
    struct maze *m = malloc(sizeof(struct maze*));
    if (!m) fatal("no memory for a new maze");
    m->player.x = 0;
    m->player.y = 0;
    FILE *f = fopen(filename, "r");
    if (!f) fatal("could not open maze file");
    init_maze_grid(init_maze_dimensions(m, f));
    rewind(f);
    MazeReadMap(m, f);
    fclose(f);
    return m;
}

struct maze* init_maze_dimensions(struct maze* m, FILE* f) {
    int rows = 0;
    int cols = 0;
    int colsMax = 0;
    int c = 0;
    do {
        c = fgetc(f);
        if (c == '\n') {
            if (cols > colsMax) colsMax = cols;
            cols = 0;
            ++rows;
        } else ++cols;
    } while (c != EOF);
    m->columns = colsMax;
    m->rows = rows;
    return m;
}

struct maze* init_maze_grid(struct maze* m) {
    m->grid = malloc(m->rows * sizeof(struct tile***));
    if (!m->grid) fatal("no memory for maze grid");
    for (int i = 0; i < m->rows; ++i) {
        m->grid[i] = malloc(m->columns * sizeof(struct tile**));
        if (!m->grid) fatal("no memory for maze grid");
        for (int j = 0; j < m->columns; ++j) {
            m->grid[i][j] = new_tile();
            if (!m->grid[i][j]) fatal("no memory for new tile");
        }
    }
    return m;
}

void delete_maze(struct maze* m) {
    // TODO free everything
    free(m);
}

// Reads the characters into the maze grid 2D array
void MazeReadMap(struct maze* m, FILE* f) {
    int c;
    struct point p = {0,0};
    do {
        c = fgetc(f);
        switch (c) {
            case '-':
            case '|':
                MazeSetTile(m, p, wall, c);
                break;
            case '\n':
                p.x = -1;
                ++p.y;
                break;
            case PLAYER_CHAR:
                MazeSetPlayer(m, p);
                MazeSetTile(m, p, floor, '.');
                break;
            default:
                MazeSetTile(m, p, floor, '.');
        }
        ++p.x;
    } while (c != EOF && p.y < m->rows);
}

void MazePrintMap(struct maze* m) {
    struct point p = {0,0};
    for (; p.y < m->rows; ++p.y)
        for (p.x = 0; p.x < m->columns; ++p.x)
            TuiPrint(
                    p,
                    p.x == m->player.x &&
                    p.y == m->player.y
                      ? PLAYER_CHAR
                      : m->grid[p.y][p.x]->character);
}

struct maze* MazeSetPlayer(struct maze* m, struct point p) {
    m->player.x = p.x;
    m->player.y = p.y;
    return m;
}

struct maze* MazeSetTile(struct maze* m, struct point p, enum TileType t, int c) {
    if (p.y > m->rows) fatal("set point rows out of bounds");
    if (p.x > m->columns) fatal("set point columns out of bounds");
    m->grid[p.y][p.x]->type = t;
    m->grid[p.y][p.x]->character = c;
    return m;
}
