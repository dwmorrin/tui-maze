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
    quit = 'q',
    ignore = ';',
    inventory = 'i',
    movement = 'm',
    use = 'u'
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
void GamePlayerEat(struct game*);
int GamePlayerMove(struct game*, enum move);
int GameBattle(struct game*, struct maze*, int x, int y, int mv);
int GameAddItem(struct game *g, struct item *it);
void GameReset(struct game*);
void GameModeSet(struct game*, enum GameMode);
void GameInventorySelect(struct game*, enum move);
void GamePlayerUseItem(struct game*);
void GamePrintStats(struct game*);

#endif
