#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"
#include "maze.h"
#include "point.h"
#include "tui.h"

struct maze* new_maze(const char* filename) {
    struct maze *m = (struct maze*)malloc(sizeof(struct maze*));
    if (!m) fatal("no memory for a new maze");
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
    m->grid = (int**)malloc(m->rows * sizeof(int*));
    for (int i = 0; i < m->rows; ++i) {
        m->grid[i] = (int*)malloc(m->columns * sizeof(int*));
        if (!m->grid[i]) fatal("no memory for new maze row");
        for (int j = 0; j < m->columns; ++j) m->grid[i][j] = 0;
    }
    return m;
}

void delete_maze(struct maze* m) {
    for (int i = 0; i < m->rows; ++i) free(m->grid[i]);
    free(m->grid);
    free(m);
}

void MazeReadMap(struct maze* m, FILE* f) {
    int c;
    struct point p = {0,0};
    do {
        c = fgetc(f);
        switch (c) {
            case '-':
            case '|':
                  TuiPrint(&p, c);
                  MazeSetPoint(m, p, c);
                  break;
            case '\n':
                p.x = -1;
                ++p.y;
                break;
        }
        ++p.x;
    } while (c != EOF && p.y < m->rows);
}

struct maze* MazeSetPoint(struct maze* m, struct point p, int c) {
    if (p.y > m->rows) fatal("set point rows out of bounds");
    if (p.x > m->columns) fatal("set point columns out of bounds");
    m->grid[p.y][p.x] = c;
    return m;
}
