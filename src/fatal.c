#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"

void fatal(const char* message) {
    endwin(); // exit curses mode so terminal is not is a strange state
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}
