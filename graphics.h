#ifndef _GUI_H_
#define _GUI_H_


#include <GL/gl.h>
#include <GL/glu.h>

#include "substratum.h"
#include "vector3.h"
#include "SDL.h"
#include <vector>


/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640/2
#define SCREEN_HEIGHT 480/2
#define SCREEN_BPP     16


#define MAX_FPS 100
#define MIN_MSPF 1000/MAX_FPS


using namespace std;


class gui {

 public:
  gui();
  ~gui();

  void resizeWindow( int width, int height );


  void drawBox();

  void drawSubstratum(  substratum * obj );

  void drawScene( const substratum ** objs, int numObjs );
  void drawScene( const vector< substratum *>* objs );

  void toggleFullScreen();

  void moveCamera( float x, float y, float z );

  void rotateCamera( float x, float y, float z );

  void setCamera( vector3 pos, vector3 rot );


 private:

  GLuint texFloor;		//This is our texture


  /* This is our SDL surface */
  SDL_Surface *surface;

  /* Flags to pass to SDL_SetVideoMode */
  int videoFlags;


  /* Display List Box ID */
  GLuint boxDL;
  
  GLuint floorDL;
  GLuint boxShadowDL;

  vector3 cameraPos;
  vector3 cameraRot;


  void initGL( GLvoid );
  GLuint createBox();
  void createFloor();

};

#endif
