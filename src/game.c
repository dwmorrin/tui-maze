#include <stdio.h>
#include <stdlib.h>

#include "actor.h"
#include "fatal.h"
#include "game.h"
#include "items.h"
#include "inventory.h"
#include "maze.h"
#include "tui.h"

int roll_die(int sides) {
    return (rand() % sides) + 1;
}

struct game* new_game(int levels_length) {
  struct game *g = malloc(sizeof(struct game));
  g->level = 0;
  g->levels_length = levels_length;
  g->player = new_actor('@');
  g->inventory = new_inventory();
  g->levels = malloc(levels_length * sizeof(struct maze*));
  for (int i = 0; i < levels_length; ++i) g->levels[i] = new_maze(i);
  g->player->p.x = g->levels[0]->start.x;
  g->player->p.y = g->levels[0]->start.y;
  return g;
}

void delete_game(struct game *g) {
  for (int i = 0; i < g->levels_length; ++i) delete_maze(g->levels[i]);
  delete_inventory(g->inventory);
  delete_actor(g->player);
  free(g);
}

void GamePrintLevel(struct game *g) {
  MazePrintMap(g->levels[g->level], g->player);
}

void GamePrintInventory(struct game* g) {
    struct point p = {0, g->levels[g->level]->rows +1};
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        TuiPrint(p, '|');
        ++p.x;
        TuiPrint(
            p,
            g->inventory[i]->type == noitem
            ? ' '
            : g->inventory[i]->character
        );
        ++p.x;
    }
    TuiPrint(p, '|');
}

// returns index of item or -1 if inventory full
int GameAddItem(struct game *g, struct item *it) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (g->inventory[i]->type == noitem) {
            copy_item(it, g->inventory[i]);
            GamePrintInventory(g);
            return i;
        }
    }
    return -1;
}

int GamePlayerEat(struct game *g, int mv) {
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (g->inventory[i]->type == food) {
            if (g->player->hp >= 10) {
                TuiPopup("you are not hungry");
                return mv;
            }
            g->player->hp += g->inventory[i]->value;
            clear_item(g->inventory[i]);
            GamePrintInventory(g);
            TuiPopup("yum");
            return mv;
        }
    }
    TuiPopup("you have no food");
    return mv;
}

// return quit to kill game
int GamePlayerMove(struct game *g, enum move mv) {
    int x = g->player->p.x;
    int y = g->player->p.y;
    struct maze *m = g->levels[g->level];
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
        case stairs:
            g->level = m->grid[y][x]->character - '0';
            if (g->level < 0 || g->level >= g->levels_length) fatal("level out of bounds");
            // return player here if they come back
            // assumes only one way to return
            m->start.x = x;
            m->start.y = y;
            // switch levels, update player position
            m = g->levels[g->level];
            g->player->p.x = m->start.x;
            g->player->p.y = m->start.y;
            // erase current level
            TuiClear();
            break;
        case false_wall:
            TuiPopup("Secret passage!");
            g->player->p.x = x;
            g->player->p.y = y;
            break;
        case floor:
            switch (m->grid[y][x]->what) {
                case none:
                    g->player->p.x = x;
                    g->player->p.y = y;
                    break;
                case coins: {
                    int coins_found = roll_die(20);
                    g->player->coins += coins_found;
                    char msg[80];
                    sprintf(msg, "You found %d coins", coins_found);
                    TuiPopup(msg);
                    m->grid[y][x]->what = none;
                    g->player->p.x = x;
                    g->player->p.y = y;
                    break;
                }
                case item: {
                    g->player->p.x = x;
                    g->player->p.y = y;
                    int i = GameAddItem(g, m->grid[y][x]->item_ref);
                    if (i < 0) {
                        TuiPopup(
                            "You found an item but "
                            "your inventory is full."
                        );
                    } else {
                        // grid no longer owns item
                        delete_item(m->grid[y][x]->item_ref);
                        m->grid[y][x]->item_ref = NULL;
                        GamePlayerItemEffect(g, i);
                        m->grid[y][x]->what = none;
                        char name[80];
                        sprintf(name, "You found an item: %s", item_name(g->inventory[i]));
                        TuiPopup(name);
                    }
                    break;
                }
                case actor:
                    mv = GameBattle(g, m, x, y, mv);
                    if (mv == quit) return mv;
                    break;
            }
            break;
        case pit:
            TuiPopup("you fall into an endless void");
            g->player->p.x = x;
            g->player->p.y = y;
            return quit;
        case wall:
            // flash or bell
            TuiPopup("Ouch (you walked into a wall)");
            break;
    }
    MazePrintMap(m, g->player);
    return mv;
}

int GameBattle(struct game *g, struct maze *m, int x, int y, int mv) {
    struct actor *e = m->grid[y][x]->actor_ref;
    int attack = roll_die(e->attack);
    int defend = roll_die(g->player->attack);
    e->hp -= defend;
    g->player->hp -= attack;
    if (g->player->hp <= 0) {
        TuiPopup("you died!");
        return quit;
    }
    int won = e->hp <= 0;
    char msg[80];
    sprintf(
        msg,
        "%s(%d)! you roll %d, they roll %d, you %s",
        actor_name(e),
        e->hp,
        defend,
        attack,
        won ? "won" : "do some damage"
    );
    TuiPopup(msg);
    if (won) {
        int i = GameAddItem(g, e->weapon);
        if (i < 0) {
            TuiPopup(
                "You found an item but "
                "your inventory is full."
            );
            // TODO transfer item to grid
        } else {
            // remove item ownership
            free(e->weapon);
            e->weapon = NULL;
            TuiPopup("You got a weapon");
        }
        i = GameAddItem(g, e->food);
        if (i < 0) {
            TuiPopup(
                "You found food but "
                "your inventory is full."
            );
        } else {
            // remove item ownership
            free(e->food);
            e->food = NULL;
            TuiPopup("You got food");
        }
        g->player->coins += e->coins;
        m->grid[y][x]->what = none;
        // TODO delete actor
    }
    return mv;
}

void GamePlayerItemEffect(struct game *g, int i) {
    switch (g->inventory[i]->type) {
        case sword:
            g->player->attack += g->inventory[i]->value;
            break;
        default:
            break;
    }
}

void GameReset(struct game *g) {
    delete_game(g);
    g = new_game(2);
    GamePrintLevel(g);
}

