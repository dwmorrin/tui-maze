#ifndef GAME
#define GAME

#include "actor.h"
#include "items.h"
#include "maze.h"

enum move {
    left = 'a',
    right = 'd',
    down = 's',
    up = 'w'
};

enum action {
    eat = 'e',
    quit = 'q',
    ignore = ';',
    inventory = 'i',
    movement = 'm'
};

enum GameMode {
    MapMode,
    InventoryMode
};

struct game {
  int level;
  int levels_length;
  enum GameMode mode;
  int inventory_index;
  struct actor *player;
  struct item **inventory;
  struct maze **levels;
};

struct game* new_game(int levels_length);
void delete_game(struct game*);

void GamePrintLevel(struct game*);
void GamePrintInventory(struct game*);
int GamePlayerEat(struct game*, int mv);
int GamePlayerMove(struct game*, enum move);
int GameBattle(struct game*, struct maze*, int x, int y, int mv);
void GamePlayerItemEffect(struct game*, int);
int GameAddItem(struct game *g, struct item *it);
void GameReset(struct game*);
void GameModeSet(struct game*, enum GameMode);
void GameInventorySelect(struct game*, enum move);

#endif
