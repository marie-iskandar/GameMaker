#ifndef COMPAT_H
#define COMPAT_H

#include "GEN.H"

boolean bioskey(int dummy) {
  // TODO: SDL or something, wait for a key
  return False;
}

void wait_for_vertical_retrace() {
  // TODO: is that even a thing we should do?
}

#endif
