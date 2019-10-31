
#include <stdio.h>            /* printing */
#include <termios.h>          /* terminal I/O interface specification */
#include <unistd.h>           /* STDIN_FILENO, etc... */
#include <stdlib.h>           /* atexit */
#include <ctype.h>            /* iscntrl */
#include <string.h>           /* memcpy */
#include <sys/types.h>        /* ssize_t */
#include <fcntl.h>            /* O_CREAT */
#include <signal.h>           /* signal */
#include <getopt.h>           /* parse arguments */

#include "Editor.h"
#include "Constants.h"
#include "Termios.h"
#include "Textline.h"
#include "View.h"
#include "Display.h"

Editor niv;

/***** utility *****/

void shutdown() {
  Textline buffer;
  initTextline(&buffer);
  restoreTermios();
  write(STDOUT_FILENO, buffer.text, buffer.length);
  freeTextline(&buffer);
}

void quit(char *msg) {

  dprintf(STDOUT_FILENO, "Error: %s\n", msg);

  shutdown();
  exit(1);
}

void signalHandler(int sig) {

  if (sig == SIGSEGV) quit("caught segmentation fault");
}

/***** loop *****/

void updateDisplay() {
  int err;
  Textline buf;
  initTextline(&buf);
  appendToTextline(&buf, HIDE_CURSOR, strlen(HIDE_CURSOR));
  err = moveCursor(&buf, 0, 0);
  if (err) quit("unable to move cursor");
  appendToTextline(&buf, niv.view.display.text, niv.view.display.length);
  err = moveCursor(&buf, niv.view.x, niv.view.y);
  if (err) quit("unable to move cursor");
  appendToTextline(&buf, SHOW_CURSOR, strlen(SHOW_CURSOR));
  if (write(STDOUT_FILENO, buf.text, buf.length) < 0)
    quit("unable to draw display");
  freeTextline(&buf);
}

int getKey() {
  int count;
  char key;

  while((count = read(STDIN_FILENO, &key, 1)) != 1)
    if (count < 0) quit("unable to get user input");

  if (key == ESC) {
    char seq[4];
    seq[0] = ESC;
    count = read(STDIN_FILENO, &seq[1], 1);
    count = read(STDIN_FILENO, &seq[2], 1);
    seq[3] = '\0';
    if (strcmp(seq, ARROW_UP) == 0) return UP;
    else if (strcmp(seq, ARROW_DOWN) == 0) return DOWN;
    else if (strcmp(seq, ARROW_RIGHT) == 0) return RIGHT;
    else if (strcmp(seq, ARROW_LEFT) == 0) return LEFT;
  }

  return key;
}

int mapKey(int key) {

  char c = 0x00 | key;
  dprintf(niv.logfile, "key: %c, %d\n", c, key);

  switch (key) {
  case Ctrl('x'):
    return -1;
    break;
  default:
    commandEditor(&niv, key);
  }
  return 0;
}

int main(int argc, char **argv) {

  if (argc < 2) quit("usage: ./niv file");

  int err;
  signal(SIGSEGV, signalHandler);
  configureTermios();
  

  initEditor(&niv);
  niv.filename = argv[1];

  err = loadFileToEditor(&niv, niv.filename);
  if (err) quit("unable to open file");

  err = loadEditorDimensions(&niv);
  if (err) quit("unable to get screen dimensions");

  // for dev
  // niv.logfile = open("log", O_CREAT | O_TRUNC | O_WRONLY, 0666);
  // if (niv.logfile < 0) quit("unable to open log");
  
  int stop;
  while(1) {
    updateEditor(&niv);
    updateDisplay();
    stop = mapKey(getKey());
    if (stop) break;
  }

  Textline buffer;
  initTextline(&buffer);
  restoreTermios();
  clearDisplay(&buffer);
  moveCursor(&buffer, 0, 0);
  write(STDOUT_FILENO, buffer.text, buffer.length);
  freeTextline(&buffer);
  return 0;
}
