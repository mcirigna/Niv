#include "View.h"
#include "Textline.h"

#include <sys/ioctl.h>        /* winsize */
#include <unistd.h>           /* STDOUT_FILENO */

void initView(View *V) {
  V->rowCount = 0;
  V->colCount = 0;
  V->rowOffset = 0;
  V->colOffset = 0;
  V->x = 0;
  V->y = 0;
  initTextline(&V->display);
}

void freeView(View *V) {
  freeTextline(&V->display);
}

int loadViewDimensions(View *V) {
  struct winsize screen;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &screen) == -1 ||
      screen.ws_col == 0) {
    return -1;
  } else {
    V->rowCount = screen.ws_row;
    V->colCount = screen.ws_col;
    return 0;
  }
}

