#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"
#include "maze.h"
#include "point.h"
#include "tile.h"
#include "tui.h"
#include "enemy.h"

int roll_die(int sides) {
    return (rand() % sides) + 1;
}

enum items *new_inventory() {
    enum items *inv = malloc(sizeof(enum items) * INVENTORY_SIZE);
    if (!inv) fatal("no memory for a new inventory");
    for (int i = 0; i < INVENTORY_SIZE; ++i)
        inv[i] = noitem;
    return inv;
}

struct tile ***new_grid(struct maze *m) {
    struct tile ***g = malloc(m->rows * sizeof(struct tile**));
    if (!g) fatal("no memory for maze grid");
    // init grid
    for (int i = 0; i < m->rows; ++i) {
        g[i] = malloc(m->columns * sizeof(struct tile*));
        if (!g[i]) fatal("no memory for maze grid");
        for (int j = 0; j < m->columns; ++j) {
            g[i][j] = new_tile();
            if (!g[i][j]) fatal("no memory for new tile");
        }
    }
    return g;
}

struct enemy **new_enemies() {
    struct enemy **e = malloc(ENEMIES_SIZE * sizeof(struct enemy*));
    for (int i = 0; i < ENEMIES_SIZE; ++i) {
        e[i] = new_enemy();
    }
    return e;
}

struct maze* new_maze(const char* filename) {
    struct maze *m = malloc(sizeof(struct maze));
    if (!m) fatal("no memory for a new maze");

    // enemy list
    m->nextEnemy = 0;
    m->enemies = new_enemies();

    // inventory
    m->inventory = new_inventory();

    // counters
    m->player.x = 0;
    m->player.y = 0;
    m->coins = 0;

    // read file once to get dimensions
    FILE *f = fopen(filename, "r");
    if (!f) fatal("could not open maze file");
    init_maze_dimensions(m, f);
    if (!m->rows) fatal("map has no rows");
    if (!m->columns) fatal("map has no columns");

    // initialize maze internal grid
    m->grid = new_grid(m);
    if (!m->grid) fatal("no memory for maze grid");
    rewind(f);

    // read file again to get details into grid
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
                //TODO refactor into an ``enemy push'' fn
                if (m->nextEnemy == ENEMIES_SIZE) exit(EXIT_FAILURE);
                m->enemies[m->nextEnemy] = new_enemy();
                MazeSetTileEnemy(m,p,floor,m->enemies[m->nextEnemy]);
                ++m->nextEnemy;
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

void MazePrintInventory(struct maze* m) {
    struct point p = {0, m->rows +1};
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        TuiPrint(p, '|');
        ++p.x;
        TuiPrint(p, items_token(m->inventory[i]));
        ++p.x;
    }
    TuiPrint(p, '|');
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
    MazePrintInventory(m);
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

struct maze* MazeSetTileEnemy(struct maze* m, struct point p, enum TileType t, struct enemy *e) {
    MazeSetTile(m, p, t, e->character);
    m->grid[p.y][p.x]->what = enemy;
    m->grid[p.y][p.x]->enemy_ref = e;
    return m;
}

void MazeMessage(struct maze* m, const char* s) {
    struct point p = {0, m->rows + 2};
    TuiHLine(p, ' ', TuiColumns());
    TuiPrintLineN(p.y, s);
}

// returns index of item or -1 if inventory full
int MazeAddItem(struct maze *m, enum items it) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (m->inventory[i] == noitem) {
            m->inventory[i] = it;
            MazePrintInventory(m);
            return i;
        }
    }
    return -1;
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
            MazeMessage(m, "Secret passage!");
            m->player.x = x;
            m->player.y = y;
            break;
        case floor:
            switch (m->grid[y][x]->what) {
                case none:
                    MazeMessage(m, "");
                    m->player.x = x;
                    m->player.y = y;
                    break;
                case coins:
                    MazeMessage(m, "You got coins");
                    m->grid[y][x]->what = none;
                    m->player.x = x;
                    m->player.y = y;
                    break;
                case item: {
                    m->player.x = x;
                    m->player.y = y;
                    int i = MazeAddItem(m, sword);
                    if (i < 0) {
                        MazeMessage(
                            m,
                            "You found an item but "
                            "your inventory is full."
                        );
                    } else {
                        m->grid[y][x]->what = none;
                        MazeMessage(m, "You got an item");
                    }
                    break;
                }
                case enemy: {
                    // roll dice; if defeated move; else stay
                    char msg[80];
                    int attack = roll_die(10);
                    int defend = roll_die(10);
                    int won = defend > attack;
                    sprintf(
                        msg,
                        "Enemy! you roll %d, they roll %d, you %s",
                        defend,
                        attack,
                        won ? "won" : "lost"
                    );
                    MazeMessage(m, msg);
                    if (won) {
                        m->grid[y][x]->what = none;
                        m->grid[y][x]->character = '.';
                        m->player.x = x;
                        m->player.y = y;
                    }
                    break;
                }
            }
            break;
        case pit:
            // TODO should die
            MazeMessage(m, "");
            m->player.x = x;
            m->player.y = y;
            break;
        case wall:
            // flash or bell
            MazeMessage(m, "Ouch");
            break;
    }
    MazePrintMap(m);
    return m;
}

int items_token(enum items i) {
    switch (i) {
        case noitem:
            return ' ';
        case sword:
            return '/';
        case shield:
            return '0';
        case armor:
            return 'T';
        case food:
            return '*';
    }
}
