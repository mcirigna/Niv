#include "Constants.h"

const char ESC = '\x1b';
const char *ARROW_UP = "\x1b[A";
const char *ARROW_DOWN = "\x1b[B";
const char *ARROW_RIGHT = "\x1b[C";
const char *ARROW_LEFT = "\x1b[D";
const char *CLEAR_SCREEN = "\x1b[2J";
const char *ERASE_RIGHT_OF_LINE = "\x1b[K";
const char *MOVE_CURSOR = "\x1b[%d;%dH";
const char *HIDE_CURSOR = "\x1b[?25l";
const char *SHOW_CURSOR = "\x1b[?25h";

