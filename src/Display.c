
#include "Display.h"
#include "Textline.h"
#include "Constants.h"
#include <stdio.h>
#include <string.h>

void clearDisplay(Textline *buffer) {
  appendToTextline(buffer, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
}

int moveCursor(Textline *buffer, int x, int y) {
  char newCursorXY[16];
  int count = snprintf(newCursorXY, sizeof(newCursorXY), MOVE_CURSOR, y, x);
  if (count < 0) return -1;
  appendToTextline(buffer, newCursorXY, strlen(newCursorXY));
  return 0;
}


