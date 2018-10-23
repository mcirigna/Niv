#include "Termios.h"
#include <unistd.h>
#include <termios.h>
#include <string.h>

enum State {INIT, RESTORE};
enum State state = INIT;

struct termios new, old;

/***** private *****/

int saveTermios() {
  int err;
  err = tcgetattr(STDIN_FILENO, &old);
  return (err) ? -1 : 0;
}

/***** public *****/

int configureTermios() {

  state = INIT;
  
  int err;
  err = saveTermios();
  if (err) return -1;

  state = RESTORE;
  
  err = tcgetattr(STDIN_FILENO, &new);
  if (err) return -1;

  //    new.c_oflag &= ~(OPOST);                                      
  new.c_iflag &= ~(IXON | ICRNL | ISTRIP | BRKINT | INPCK);
  new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  new.c_cflag |= (CS8);
  new.c_cc[VMIN] = 0;
  new.c_cc[VTIME] = 1;
  
  err = tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);
  return (err) ? -1 : 0;
}

int restoreTermios() {
  int err = 0;
  if (state == RESTORE) {
    err = tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
  }
  return (err) ? -1 : 0;
}
