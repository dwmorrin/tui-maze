#include <stdlib.h>

#include "fatal.h"
#include "tile.h"

struct tile ***new_grid(int rows, int columns) {
    struct tile ***g = malloc(rows * sizeof(struct tile**));
    if (!g) fatal("no memory for maze grid");
    // init grid
    for (int i = 0; i < rows; ++i) {
        g[i] = malloc(columns * sizeof(struct tile*));
        if (!g[i]) fatal("no memory for maze grid");
        for (int j = 0; j < columns; ++j) {
            g[i][j] = new_tile();
            if (!g[i][j]) fatal("no memory for new tile");
        }
    }
    return g;
}

void delete_grid(struct tile ***g, int rows, int columns) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            delete_tile(g[i][j]);
        }
        free(g[i]);
    }
    free(g);
}

