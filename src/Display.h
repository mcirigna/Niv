#ifndef DISPLAY_H
#define DISPLAY_H

#include "Textline.h"

void clearDisplay(Textline *buffer);

int moveCursor(Textline *buffer, int x, int y);

#endif
