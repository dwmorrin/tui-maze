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
    int c = 0;
    while (c != 'q') {
        c = TuiInput();
        switch (c) {
            case 'q': {
                int response = TuiPopup("Quitting so soon? (y/n)");
                if (response != 'y') {
                    c = ' ';//something ignorable, not 'q'
                }
                break;
            }
            case up:
            case down:
            case left:
            case right:
                c = GamePlayerMove(g, c);
                break;
            case eat:
                c = GamePlayerEat(g, c);
                break;
        }
    }
    delete_game(g);
    TuiExit();
    exit(EXIT_SUCCESS);
}
