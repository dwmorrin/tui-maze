#include <stdio.h>
#include <stdlib.h>

#include "actor.h"
#include "fatal.h"
#include "game.h"
#include "grid.h"
#include "inventory.h"
#include "items.h"
#include "maze.h"
#include "point.h"
#include "tile.h"
#include "tui.h"

struct maze* new_maze(int filenumber) {
    struct maze *m = malloc(sizeof(struct maze));
    if (!m) fatal("no memory for a new maze");
    MazeSetGrid(m, filenumber);

    return m;
}

void MazeSetGrid(struct maze *m, int filenumber) {
    // currently limited to single character
    // filenames
    char filename[10];
    sprintf(filename, "%c.map.txt", filenumber + '0');
    // read file once to get dimensions
    FILE *f = fopen(filename, "r");
    if (!f) fatal("could not open maze file");
    init_maze_dimensions(m, f);
    if (!m->rows) fatal("map has no rows");
    if (!m->columns) fatal("map has no columns");

    // initialize maze internal grid
    m->grid = new_grid(m->rows, m->columns);
    if (!m->grid) fatal("no memory for maze grid");
    rewind(f);

    // read file again to get details into grid
    MazeReadMap(m, f);
    fclose(f);
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

void delete_maze(struct maze* m) {
    delete_grid(m->grid, m->rows, m->columns);
    free(m);
}

// Reads the characters into the maze grid 2D array
void MazeReadMap(struct maze* m, FILE* f) {
    int c;
    struct point p = {0,0};
    do {
        c = fgetc(f);
        switch (c) {
            case '0':
            case '1':
                MazeSetTile(m,p,stairs,c);
                break;
            case '$':
                MazeSetTileCoins(m,p);
                break;
            case MOBLIN:
            case BIG_MOBLIN:
            case GOBLIN:
            case BIG_GOBLIN:
            case BAT:
            case BIG_BAT: {
                struct actor *e = new_actor(c);
                MazeSetTileEnemy(m,p,e);
                break;
            }
            case '?': {
                struct item *anItem = new_item(sword, 3, '/');
                MazeSetTileItem(m,p,anItem);
                break;
            }
            case 'O': {
                struct item *anItem = new_item(shield, 3, c);
                MazeSetTileItem(m,p,anItem);
                break;
            }
            case 'A': {
                struct item *anItem = new_item(armor, 3, c);
                MazeSetTileItem(m,p,anItem);
                break;
            }
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
                m->start.x = p.x;
                m->start.y = p.y;
                MazeSetTile(m, p, floor, '.');
                break;
            case '.':
                MazeSetTile(m, p, floor, c);
                break;
            default:
                MazeSetTile(m, p, pit, ' ');
        }
        ++p.x;
    } while (c != EOF && p.y < m->rows);
}

/**
 * To determine if maze tile is invisible to player
 */
int MazePlayerTileInvisible(struct maze *m, struct actor *player, struct point p) {
    const int d = 3;
    const int px = player->p.x;
    const int py = player->p.y;
    return (
        p.x > px + d ||
        p.x < px - d ||
        p.y > py + d ||
        p.y < py - d
    );
}

void MazePrintMap(struct maze *m, struct actor *player) {
    struct point p = {0,0};
    for (; p.y < m->rows; ++p.y)
        for (p.x = 0; p.x < m->columns; ++p.x)
            if (MazePlayerTileInvisible(m, player, p)) {
                TuiPrint(p, ' ');
            } else if (
                p.x == player->p.x &&
                p.y == player->p.y
            ) {
                TuiPrint(p, PLAYER_CHAR);
            } else {
                struct tile *t = m->grid[p.y][p.x];
                enum color_pair c = tile_color(t);
                TuiColor(c, 1);
                TuiPrint(p, tile_character(t));
                TuiColor(c, 0);
            }
}

struct maze* MazeSetTile(
    struct maze* m,
    struct point p,
    enum TileType t,
    int c
) {
    if (p.y > m->rows) fatal("set point rows out of bounds");
    if (p.x > m->columns) fatal("set point columns out of bounds");
    m->grid[p.y][p.x]->type = t;
    m->grid[p.y][p.x]->character = c;
    return m;
}

struct maze* MazeSetTileCoins(struct maze* m, struct point p) {
    MazeSetTile(m, p, floor, '.');
    m->grid[p.y][p.x]->what = coins;
    return m;
}

struct maze* MazeSetTileItem(
    struct maze* m,
    struct point p,
    struct item* i
) {
    MazeSetTile(m, p, floor, '.');
    m->grid[p.y][p.x]->what = item;
    m->grid[p.y][p.x]->item_ref = i;
    return m;
}

struct maze* MazeSetTileEnemy(
    struct maze* m,
    struct point p,
    struct actor *e
) {
    MazeSetTile(m, p, floor, '.');
    m->grid[p.y][p.x]->what = actor;
    m->grid[p.y][p.x]->actor_ref = e;
    return m;
}

void MazeMessage(struct maze* m, const char* s) {
    struct point p = {0, m->rows + 2};
    TuiHLine(p, ' ', TuiColumns());
    TuiPrintLineN(p.y, s);
}

void MazeMessagePause(struct maze* m, const char* s) {
    struct point p = {0, m->rows + 2};
    TuiHLine(p, ' ', TuiColumns());
    TuiPrintLineNAndPause(p.y, s);
}

struct tile *MazeFindAdjacentEmptyFloor(struct maze *m, struct point p) {
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            // skip if no offset
            if (i == 0 && j == 0) continue;
            int yy = p.y + i;
            int xx = p.x + j;
            if (yy < 0 || yy > m->rows) continue;
            if (xx < 0 || xx > m->columns) continue;
            if (
                TileIsEmptyFloor(m->grid[yy][xx]) &&
                roll_die(6) > 3
            ) return m->grid[yy][xx];
        }
    }
    return NULL;
}

void MazeMoveActors(struct maze *m) {
    struct point p = {0,0};
    for (; p.y < m->rows; ++p.y) {
        for (p.x = 0; p.x < m->columns; ++p.x) {
            struct tile *t = m->grid[p.y][p.x];
            if (t->what == actor) {
                // find floor
                struct tile *tt = MazeFindAdjacentEmptyFloor(m, p);
                if (!tt) continue;
                // switch actor from t to tt
                tt->actor_ref = t->actor_ref;
                tt->what = actor;
                t->what = none;
                t->actor_ref = NULL;
            }
        }
    }
}
