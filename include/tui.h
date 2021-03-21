#ifndef TUI
#define TUI

#include "point.h"

int TuiColumns();
void TuiExit();
void TuiHLine(struct point, int);
void TuiInit();
int TuiInput();
void TuiPrintCenter(const char*);
void TuiPrint(struct point, int);
int TuiRows();
void TuiVLine(struct point, int);
void TuiPrintLineN(int, const char*);

#endif
