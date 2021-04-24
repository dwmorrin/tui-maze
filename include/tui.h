#ifndef TUI
#define TUI

#include "point.h"

void TuiClear();
int TuiColumns();
void TuiExit();
void TuiHLine(struct point, int, int);
void TuiInit();
int TuiInput();
void TuiPrintCenter(const char*);
void TuiPrint(struct point, int);
int TuiRows();
void TuiVLine(struct point, int);
void TuiPrintLineN(int, const char*);
void TuiPrintLineNAndPause(int, const char*);
int TuiPopup(const char*);

#endif
