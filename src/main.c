#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "maze.h"
#include "point.h"
#include "tile.h"
#include "tui.h"

int main() {
    srand(time(NULL)); // initialize PRNG for rolling dice
    TuiInit();

    struct game* g = new_game(2);
    GamePrintLevel(g);
    TuiPopup("MAZE (press any key)");
    int c = ignore;
    while (c != quit) {
        c = TuiInput();
        switch (c) {
            case quit: {
                int response = TuiPopup("Quitting so soon? (y/n)");
                if (response != 'y') {
                    c = ignore;
                }
                break;
            }
            case up:
            case down:
            case left:
            case right:
                if (g->mode == MapMode) {
                    c = GamePlayerMove(g, c);
                    if (c == quit) {
                        c = ignore;
                        GameReset(g);
                    }
                } else if (g->mode == InventoryMode) {
                    GameInventorySelect(g, c);
                }
                break;
            case eat:
                c = GamePlayerEat(g, c);
                break;
            case inventory:
                GameModeSet(g, InventoryMode);
                break;
            case movement:
                GameModeSet(g, MapMode);
                break;
        }
    }
    delete_game(g);
    TuiExit();
    exit(EXIT_SUCCESS);
}
