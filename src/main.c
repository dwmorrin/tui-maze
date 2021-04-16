#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "point.h"
#include "tui.h"

int main() {
    srand(time(NULL)); // initialize PRNG for rolling dice
    TuiInit();
    struct maze* m = new_maze('0');
    MazePrintMap(m);
    int c = 0;
    while (c != 'q') {
        c = TuiInput();
        switch (c) {
            case up:
            case down:
            case left:
            case right:
                c = MazeMovePlayer(m, c);
                break;
            case eat:
                c = MazePlayerEat(m, c);
                break;
        }
    }
    delete_maze(m);
    TuiExit();
    exit(EXIT_SUCCESS);
}
