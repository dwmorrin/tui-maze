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
    eat = 'e'
};

struct game {
  int level;
  int levels_length;
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

#endif