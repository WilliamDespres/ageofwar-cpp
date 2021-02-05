#include "io/conio.hpp"

/*
This file simulates the Windows "conio.h" non-standard header for Linux.
It allows to catch single key press events without pressing enter.
See https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
for more information.
*/
struct termios orig_termios;

void reset_terminal_mode() {
  tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode() {
  struct termios new_termios;

  /* take two copies - one for now, one for later */
  tcgetattr(0, &orig_termios);
  memcpy(&new_termios, &orig_termios, sizeof(new_termios));

  /* register cleanup handler, and set the new terminal mode */
  atexit(reset_terminal_mode);
  cfmakeraw(&new_termios);
  tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit() {
  set_conio_terminal_mode();
  struct timeval tv = { 0L, 0L };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  auto res = select(1, &fds, NULL, NULL, &tv);
  reset_terminal_mode();
  return res;
}

int getch() {
  set_conio_terminal_mode();
  int r;
  unsigned char c;
  if ((r = read(0, &c, sizeof(c))) < 0) {
      reset_terminal_mode();
      return r;
  } else {
      reset_terminal_mode();
      return c;
  }
}