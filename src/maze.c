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
            case '$':
                MazeSetTileWhat(m,p,floor,'.',coins);
                break;
            case '!':
                MazeSetTileWhat(m,p,floor,'.',enemy);
                break;
            case '?':
                MazeSetTileWhat(m,p,floor,'.',item);
                break;
            case '-':
            case '|':
                MazeSetTile(m, p, wall, c);
                break;
            case '/':
                MazeSetTile(m, p, false_wall, '|');
                break;
            case '_':
                MazeSetTile(m, p, false_wall, '-');
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

struct maze* MazeSetTileWhat(struct maze* m, struct point p, enum TileType t, int c, enum Stuff s) {
    MazeSetTile(m, p, t, c);
    m->grid[p.y][p.x]->what = s;
    return m;
}

struct maze* MazeMovePlayer(struct maze* m, enum move mv) {
    int x = m->player.x;
    int y = m->player.y;
    switch (mv) {
        case up:
            if (y == 0) return m;
            --y;
            break;
        case down:
            if (y == m->rows - 1) return m;
            ++y;
            break;
        case left:
            if (x == 0) return m;
            --x;
            break;
        case right:
            if (x == m->columns - 1) return m;
            ++x;
            break;
    }
    switch (m->grid[y][x]->type) {
        case false_wall:
            TuiPrintLineN(m->rows + 2, "Secret passage!");
            m->player.x = x;
            m->player.y = y;
            MazePrintMap(m);
            break;
        case floor:
            switch (m->grid[y][x]->what) {
                case none:
                    TuiPrintLineN(m->rows + 2, "               ");
                    break;
                case coins:
                    TuiPrintLineN(m->rows + 2, "You got coins  ");
                    m->grid[y][x]->what = none;
                    break;
                case item:
                    TuiPrintLineN(m->rows + 2, "You got an item");
                    m->grid[y][x]->what = none;
                    break;
                case enemy:
                    TuiPrintLineN(m->rows + 2, "An enemy attack");
                    break;
            }
            m->player.x = x;
            m->player.y = y;
            MazePrintMap(m);
            break;
        case pit:
            // TODO should die
            TuiPrintLineN(m->rows + 2, "               ");
            m->player.x = x;
            m->player.y = y;
            MazePrintMap(m);
            break;
        case wall:
            // flash or bell
            TuiPrintLineN(m->rows + 2, "Ouch           ");
            break;
    }
    return m;
}
