#ifndef CONSTANTS_H
#define CONSTANTS_H

#define Ctrl(key) (key & 0x1f)

const char ESC;
const char *ARROW_UP;
const char *ARROW_DOWN;
const char *ARROW_RIGHT;
const char *ARROW_LEFT;
const char *CLEAR_SCREEN;
const char *ERASE_RIGHT_OF_LINE;
const char *MOVE_CURSOR;
const char *HIDE_CURSOR;
const char *SHOW_CURSOR;

enum Command {
	      CARRIAGE_RETURN = Ctrl('m'),
	      BACKSPACE = Ctrl('h'),
	      DELETE = 127,
	      UP = 1000,
	      DOWN,
	      LEFT,
	      RIGHT
};

#endif
