#ifndef _EVENTS_H_
#define _EVENTS_H_


#include "SDL.h"
#include "graphics.h"



#define WALK_DELTA 0.05f
#define ROT_DELTA 0.3f


class events {


 public:
  events(graphics* gr);

  void handleEvents();

  void setCreaturePos( vector3 pos );

  /* determines if the application window is active or
     is in background  */
  bool isActive;

  /* Some quit request ? */
  bool wantQuit;

  bool show; // want to show the creature redered in screen ?

  bool slow;


 private:
  vector3 creaturePos;

  graphics* g;

  /* store keyboard state */
  bool keyboard[SDLK_LAST];


  void handleKeyPress( SDL_keysym *keysym );
  void handleKeyUp( SDL_keysym *keysym );
  void handleKeys();

};






#endif
