#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

#include "events.h"
#include "graphics.h"






void events::handleKeyPress( SDL_keysym *keysym ){

  if( keysym->sym == SDLK_F1 )
    g->toggleFullScreen();

  if( keysym->sym == SDLK_F2 )
    this->show = !this->show;

  keyboard[keysym->sym] = true;

  if( keyboard[SDLK_p] )
    slow = !slow;
}


void events::handleKeyUp( SDL_keysym *keysym ){
  keyboard[keysym->sym] = false;
}



void events::setCreaturePos( vector3 pos ){
  creaturePos = pos;
}


void events::handleKeys(){
  float walk_delta, rot_delta;

  walk_delta = WALK_DELTA;
  rot_delta = ROT_DELTA;


  if( keyboard[SDLK_g] ){
    g->setCamera( (vector3(0.f,-15.f,-15.f) - creaturePos), vector3(45.0f,0.f,0.f));
  }


  if( keyboard[SDLK_UP] )
    g->moveCamera(0,0,-walk_delta);

    
  if( keyboard[SDLK_LSHIFT] ){
    walk_delta *= 2;
    rot_delta *=2;
  }

  if( keyboard[SDLK_ESCAPE] ){
    if( !show )
      wantQuit = true;
    else
      show = false;
  }


  if( keyboard[SDLK_UP] )
    g->moveCamera(0,0,-walk_delta);

  if( keyboard[SDLK_DOWN] )
    g->moveCamera(0,0,walk_delta);

  if( keyboard[SDLK_a] )
    g->moveCamera(walk_delta,0,0);

  if( keyboard[SDLK_d] )
    g->moveCamera(-walk_delta,0,0);

  if( keyboard[SDLK_w] )
    g->moveCamera(0,-walk_delta,0);

  if( keyboard[SDLK_s] )
    g->moveCamera(0,walk_delta,0);


  if( keyboard[SDLK_z] )
    g->rotateCamera(-rot_delta,0,0);

  if( keyboard[SDLK_x] )
    g->rotateCamera(rot_delta,0,0);

  if( keyboard[SDLK_LEFT] )
    g->rotateCamera(0,-rot_delta,0);

  if( keyboard[SDLK_RIGHT] )
    g->rotateCamera(0,rot_delta,0);

}



 events::events(graphics* gr)
{
  slow = false;
  this->show = true;
  isActive = true;
  wantQuit = false;
  g = gr;
  int i;
  for( i=0; i < SDLK_LAST; i++)
    keyboard[i] = false;
}



void events::handleEvents(){

  /* used to collect events */
  SDL_Event event;

  /* handle the events in the queue */

  while ( SDL_PollEvent( &event ) )
    {
      switch( event.type )
	{
	case SDL_ACTIVEEVENT:
	  /* Something's happend with our focus
	   * If we lost focus or we are iconified, we
	   * shouldn't draw the screen
	   */
	  if ( event.active.gain == 0 )
	    isActive = false;
	  else
	    isActive = true;
	  break;			    
	case SDL_VIDEORESIZE:
	  /* handle resize event */
	  g->resizeWindow( event.resize.w, event.resize.h );
	  break;
	case SDL_KEYDOWN:
	  /* handle key presses */
	  handleKeyPress( &event.key.keysym );
	  break;
	case SDL_KEYUP:
	  /* handle key presses */
	  handleKeyUp( &event.key.keysym );
	  break;
	case SDL_QUIT:
	  /* handle quit requests */
	  wantQuit = true;
	  break;
	default:
	  break;
	}
    }
  handleKeys();

  //if( slow )
  //SDL_Delay(10);
  
}
