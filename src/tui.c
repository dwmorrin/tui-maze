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
    start_color();
    init_pair(blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(red, COLOR_RED, COLOR_BLACK);
    init_pair(yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(white, COLOR_WHITE, COLOR_BLACK);
    init_pair(red_select, COLOR_WHITE, COLOR_RED);
}

void TuiColor(enum color_pair c, int on) {
    if (on) attron(COLOR_PAIR(c));
    else attroff(COLOR_PAIR(c));
}

void TuiClear() {
    erase();
    refresh();
}

void TuiPrintCenter(const char *s) {
    mvprintw(LINES/2, COLS/2 - strlen(s)/2, "%s", s);
}

void TuiPrintLineN(int n, const char *s) {
    mvprintw(n, 0, "%s", s);
}

void TuiPrintLineNAndPause(int n, const char *s) {
    mvprintw(n, 0, "%s (press any key)", s);
    TuiInput();
    move(n, 0);
    hline(' ', COLS);
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
    endwin();
    exit(EXIT_SUCCESS);
}

void TuiPrint(struct point p, int c) {
    mvaddch(p.y, p.x, c);
}

int TuiPopup(const char *msg) {
    WINDOW *popup = newwin(4, COLS - 4, 2, 2);
    wborder(popup, 0,0,0,0,0,0,0,0);
    mvwaddstr(popup, 2, 2, msg);
    wrefresh(popup);
    int response = wgetch(popup);
    werase(popup);
    wrefresh(popup);
    delwin(popup);
    touchwin(stdscr);
    refresh();
    return response;
}
