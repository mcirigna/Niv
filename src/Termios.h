#ifndef TERMIOS_H
#define TERMIOS_H

/* 0 if success, -1 otherwise */
int configureTermios();

/* 0 if success, -1 otherwise */
int restoreTermios();

#endif
