#include <stdlib.h>

#include "maze.h"
#include "point.h"
#include "tui.h"

int main() {
    TuiInit();
    struct maze* m = new_maze("map.txt");
    MazePrintMap(m);
    int c = 0;
    while (c != 'q') {
        c = TuiInput();
        switch (c) {
            case up:
            case down:
            case left:
            case right:
                MazeMovePlayer(m, c);
        }
    }
    delete_maze(m);
    TuiExit();
    exit(EXIT_SUCCESS);
}
