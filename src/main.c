#include <stdlib.h>

#include "maze.h"
#include "point.h"
#include "tui.h"

int main() {
    TuiInit();
    struct maze* m = new_maze("map.txt");
    MazePrintMap(m);
    delete_maze(m);
    TuiExit();
    exit(EXIT_SUCCESS);
}
