#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"
#include "maze.h"
#include "point.h"
#include "tui.h"

struct maze* new_maze(int rows, int cols) {
    struct maze *m = (struct maze*)malloc(sizeof(struct maze*));
    if (!m) fatal("no memory for a new maze");
    m->rows = rows;
    m->columns = cols;
    m->grid = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        m->grid[i] = (int*)malloc(cols * sizeof(int*));
        if (!m->grid[i]) fatal("no memory for new maze row");
        for (int j = 0; j < cols; ++j) m->grid[i][j] = 0;
    }
    return m;
}

void delete_maze(struct maze* m) {
    for (int i = 0; i < m->rows; ++i) free(m->grid[i]);
    free(m->grid);
    free(m);
}

void MazeRead(struct maze* m) {
    FILE *f = fopen("map.txt", "r");
    if (!f) fatal("could not open maze file");
    int c;
    struct point p = {0,0};
    do {
        c = fgetc(f);
        switch (c) {
            case '-':
            case '|':
                  TuiPrint(&p, c);
                  if (p.x >= m->columns)
                      fatal("maze columns mismatch");
                  m->grid[p.y][p.x] = c;
                  break;
            case '\n':
                p.x = -1;
                ++p.y;
                break;
        }
        ++p.x;
    } while (c != EOF && p.y < m->rows);
    fclose(f);
}
