#ifndef COMPAT_H
#define COMPAT_H

#include "SDL.h"

#include "GEN.H"

boolean bioskey(int isNonBlocking) {
  SDL_Event event;

  if (isNonBlocking) {
    while (SDL_PollEvent(&event)) {  // poll until all events are handled!
      // decide what to do with this event.
      if (event.type == SDL_KEYUP) {
        return True;
      }
    }
    return False;
  }

  if (SDL_WaitEvent(&event)){
    if(event.type == SDL_KEYUP){
      return True;
    }
  }
  return False;
}

void wait_for_vertical_retrace() {
  // TODO: is that even a thing we should do?
}

#endif
