#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "tui.h"

int TuiColumns() { return COLS; }
int TuiRows() { return LINES; }

int TuiInput() {
    return getch();
}

void TuiInit() {
    initscr();
    curs_set(false);
    noecho();
}

void TuiPrintCenter(const char *s) {
    mvprintw(LINES/2, COLS/2 - strlen(s)/2, "%s", s);
}

void TuiPrintLineN(int n, const char *s) {
    mvprintw(n, 0, "%s", s);
}

void TuiHLine(struct point p, int c, int length) {
    move(p.y, p.x);
    hline(c, length);
}

void TuiVLine(struct point p, int length) {
    move(p.y, p.x);
    vline('|', length);
}

void TuiExit() {
    TuiPrintCenter("press any key");
    refresh();
    getch();
    endwin();
    exit(EXIT_SUCCESS);
}

void TuiPrint(struct point p, int c) {
    mvaddch(p.y, p.x, c);
}
