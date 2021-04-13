#include <stdio.h>
#include <stdlib.h>

#include "grid.h"
#include "fatal.h"
#include "inventory.h"
#include "items.h"
#include "maze.h"
#include "point.h"
#include "tile.h"
#include "tui.h"
#include "actor.h"

int roll_die(int sides) {
    return (rand() % sides) + 1;
}

struct maze* new_maze(const char* filename) {
    struct maze *m = malloc(sizeof(struct maze));
    if (!m) fatal("no memory for a new maze");

    // inventory
    m->inventory = new_inventory();

    m->player = new_actor('@');

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
    delete_grid(m->grid, m->rows, m->columns);
    delete_actor(m->player);
    delete_inventory(m->inventory);
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
                MazeSetTileCoins(m,p);
                break;
            case MOBLIN:
            case BIG_MOBLIN:
            case GOBLIN:
            case BIG_GOBLIN:
            case BAT:
            case BIG_BAT: {
                struct actor *e = new_actor(c);
                MazeSetTileEnemy(m,p,floor,e);
                break;
            }
            case '?': {
                struct item *anItem = new_item(sword, 3, '/');
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
            TuiPrint(
                p,
                m->inventory[i]->type == noitem
                ? ' '
                : m->inventory[i]->character
            );
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
                        p.x == m->player->p.x &&
                        p.y == m->player->p.y
                          ? PLAYER_CHAR
                          : tile_character(m->grid[p.y][p.x]));
        MazePrintInventory(m);
        MazeStats(m);
    }

    struct maze* MazeSetPlayer(struct maze* m, struct point p) {
        m->player->p.x = p.x;
        m->player->p.y = p.y;
        return m;
    }

    struct maze* MazeSetTile(struct maze* m, struct point p, enum TileType t, int c) {
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

    struct maze* MazeSetTileItem(struct maze* m, struct point p, struct item* i) {
        MazeSetTile(m, p, floor, '.');
        m->grid[p.y][p.x]->what = item;
        m->grid[p.y][p.x]->item_ref = i;
    return m;
}
//TODO type should not be passed, just hardcode as actor
struct maze* MazeSetTileEnemy(struct maze* m, struct point p, enum TileType t, struct actor *e) {
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

void MazeStats(struct maze* m) {
    struct point p = {0, m->rows + 4};
    TuiHLine(p, ' ', TuiColumns());
    char stats[80];
    sprintf(
        stats,
        "Health: %d, Attack: %d",
        m->player->hp,
        m->player->attack
    );
    TuiPrintLineN(p.y, stats);
}

// returns index of item or -1 if inventory full
int MazeAddItem(struct maze *m, struct item* it) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (m->inventory[i]->type == noitem) {
            // copy item
            m->inventory[i]->type = it->type;
            m->inventory[i]->value = it->value;
            m->inventory[i]->character = it->character;
            MazePrintInventory(m);
            return i;
        }
    }
    return -1;
}

void MazePlayerItemEffect(struct maze *m, int i) {
    switch (m->inventory[i]->type) {
        case sword:
            m->player->attack += m->inventory[i]->value;
            break;
        default:
            break;
    }
}

// return 'q' to kill game
int MazeMovePlayer(struct maze* m, enum move mv) {
    int x = m->player->p.x;
    int y = m->player->p.y;
    switch (mv) {
        case up:
            if (y == 0) return mv;
            --y;
            break;
        case down:
            if (y == m->rows - 1) return mv;
            ++y;
            break;
        case left:
            if (x == 0) return mv;
            --x;
            break;
        case right:
            if (x == m->columns - 1) return mv;
            ++x;
            break;
    }
    switch (m->grid[y][x]->type) {
        case false_wall:
            MazeMessage(m, "Secret passage!");
            m->player->p.x = x;
            m->player->p.y = y;
            break;
        case floor:
            switch (m->grid[y][x]->what) {
                case none:
                    MazeMessage(m, "");
                    m->player->p.x = x;
                    m->player->p.y = y;
                    break;
                case coins:
                    MazeMessage(m, "You got coins");
                    m->grid[y][x]->what = none;
                    m->player->p.x = x;
                    m->player->p.y = y;
                    break;
                case item: {
                    m->player->p.x = x;
                    m->player->p.y = y;
                    int i = MazeAddItem(m, m->grid[y][x]->item_ref);
                    if (i < 0) {
                        MazeMessage(
                            m,
                            "You found an item but "
                            "your inventory is full."
                        );
                    } else {
                        // grid no longer owns item
                        free(m->grid[y][x]->item_ref);
                        m->grid[y][x]->item_ref = NULL;
                        MazePlayerItemEffect(m, i);
                        m->grid[y][x]->what = none;
                        char name[80];
                        sprintf(name, "You found an item: %s", item_name(m->inventory[i]));
                        MazeMessage(m, name);
                    }
                    break;
                }
                case actor:
                    mv = MazeBattle(m, x, y, mv);
                    if (mv == 'q') return mv;
                    break;
            }
            break;
        case pit:
            MazeMessage(m, "you died");
            m->player->p.x = x;
            m->player->p.y = y;
            return 'q';
        case wall:
            // flash or bell
            MazeMessage(m, "Ouch");
            break;
    }
    MazePrintMap(m);
    return mv;
}

int MazeBattle(struct maze *m, int x, int y, int mv) {
    struct actor *e = m->grid[y][x]->actor_ref;
    // roll dice; if defeated move; else stay
    char msg[80];
    int attack = roll_die(e->attack);
    int defend = roll_die(m->player->attack);
    e->hp -= defend;
    m->player->hp -= attack;
    if (m->player->hp <= 0) {
        MazeMessage(m, "you died!");
        return 'q';
    }
    int won = e->hp <= 0;
    sprintf(
        msg,
        "%s(%d)! you roll %d, they roll %d, you %s",
        actor_name(e),
        e->hp,
        defend,
        attack,
        won ? "won" : "do some damage"
    );
    MazeMessage(m, msg);
    if (won) {
        int i = MazeAddItem(m, e->weapon);
        if (i < 0) {
            MazeMessage(
                m,
                "You found an item but "
                "your inventory is full."
            );
            // TODO transfer item to grid
        } else {
            // remove item ownership
            free(e->weapon);
            e->weapon = NULL;
            MazeMessage(m, "You got a weapon");
        }
        TuiInput();
        i = MazeAddItem(m, e->food);
        if (i < 0) {
            MazeMessage(
                m,
                "You found food but "
                "your inventory is full."
            );
        } else {
            // remove item ownership
            free(e->food);
            e->food = NULL;
            MazeMessage(m, "You got food");
        }
        TuiInput();
        m->grid[y][x]->what = none;
        // TODO delete actor
        m->player->p.x = x;
        m->player->p.y = y;
    }
    return mv;
}

int MazePlayerEat(struct maze *m, int mv) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (m->inventory[i]->type == food) {
            if (m->player->hp >= 10) {
                MazeMessage(m, "you are not hungry");
                return mv;
            }
            m->inventory[i]->type = noitem;
            m->player->hp += 1;
            MazePrintMap(m);
            MazeMessage(m, "yum");
            return mv;
        }
    }
    MazeMessage(m, "you have no food");
    return mv;
}
