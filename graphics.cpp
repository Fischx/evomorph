#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include <math.h>

#include "graphics.h"
#include "substratum.h"
#include "vector3.h"

#include <vector>
#include <ode/ode.h>

using namespace std;



#include "global.h"

graphics::graphics()
{

  cameraPos.assign(0.f,-10.f,-10.f);
  cameraRot.assign(45.f,0.f,0.f);

  /* this holds some info about our display */
  const SDL_VideoInfo *videoInfo;


  /* initialize SDL */
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
      fprintf( stderr, "[ %s , %d ] Video initialization failed: %s\n",
	       __FILE__, __LINE__, 
	       SDL_GetError( ) );
      this->~graphics();
      exit( 1 );
    }

  /* Fetch the video info */
  videoInfo = SDL_GetVideoInfo( );

  if ( !videoInfo )
    {
      fprintf( stderr, "[ %s , %d ] Video query failed: %s\n",
	       __FILE__, __LINE__, 
	       SDL_GetError( ) );
      this->~graphics();
      exit( 1 );
    }

  /* the flags to pass to SDL_SetVideoMode */
  videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
  videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
  videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
  videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

  /* This checks to see if surfaces can be stored in memory */
  if ( videoInfo->hw_available )
    videoFlags |= SDL_HWSURFACE;
  else
    videoFlags |= SDL_SWSURFACE;

  /* This checks if hardware blits can be done */
  if ( videoInfo->blit_hw )
    videoFlags |= SDL_HWACCEL;

  /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  /* get a SDL surface */
  surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			      videoFlags );

  /* Verify there is a surface */
  if ( !surface )
    {
      fprintf( stderr,  "[ %s , %d ]Video mode set failed: %s\n", 
	       __FILE__, __LINE__, 
	       SDL_GetError( ) );
      this->~graphics();
      exit( 1 );
    }

  /* initialize OpenGL */
  initGL( );

  /* resize the initial window */
  resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );


  /* Create the Display List of the Box */
  createBox();

  createFloor();
}


graphics::~graphics(){

  //glDeleteTextures(1, &image);

  /* clean up the window */
  SDL_Quit( );

}


/* function to reset our viewport after a window resize */
void graphics::resizeWindow( int width, int height )
{
  surface = SDL_SetVideoMode( width,
			      height,
			      SCREEN_BPP, videoFlags );
  if ( !surface )
    {
      fprintf( stderr, "[ %s , %d ] Could not get a surface after resize: %s\n",
	       __FILE__, __LINE__, SDL_GetError( ) );
      this->~graphics();
      exit( 1 );
    }


  /* Height / width ration */
  GLfloat ratio;
 
  /* Protect against a divide by zero */
  if ( height == 0 )
    height = 1;

  ratio = ( GLfloat )width / ( GLfloat )height;

  /* Setup our viewport. */
  glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

  /* change to the projection matrix and set our viewing volume. */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  /* Set our perspective */
  gluPerspective( 45.0f, ratio, 0.1f, 500.0f );

  /* Make sure we're chaning the model view and not the projection */
  glMatrixMode( GL_MODELVIEW );

  /* Reset The View */
  glLoadIdentity( );

}





/* general OpenGL initialization function */
void graphics::initGL( GLvoid )
{

  glEnable(GL_CULL_FACE);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode( GL_BACK, GL_FILL );			// Back Face Is Filled In
  glPolygonMode( GL_FRONT, GL_FILL );			// Front Face Is Drawn With Lines
  //GL_LINE

  /* Enable smooth shading */
  glShadeModel( GL_SMOOTH );

  /* Set the background black */
  glClearColor( 0.7f, 0.8f, 1.0f, 0.0f );

  /* Depth buffer setup */
  glClearDepth( 1.0f );

  /* Enables Depth Testing */
  glEnable( GL_DEPTH_TEST );


  /* The Type Of Depth Test To Do */
  glDepthFunc( GL_LEQUAL );

  /* Really Nice Perspective Calculations */
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}






GLuint graphics::createBox()
{

  boxDL = glGenLists(1);

  glNewList(boxDL,GL_COMPILE);
  

  glBegin(GL_QUADS);		/* Draw The Cube Using quads */
    glColor3f(0.0f,1.0f,0.0f);	/* Color Blue */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Top Right Of The Quad (Top) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Top Left Of The Quad (Top) */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Bottom Left Of The Quad (Top) */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Bottom Right Of The Quad (Top) */
    glColor3f(1.0f,0.5f,0.0f);	/* Color Orange */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Top Right Of The Quad (Bottom) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Top Left Of The Quad (Bottom) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Bottom Left Of The Quad (Bottom) */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Bottom Right Of The Quad (Bottom) */
    glColor3f(1.0f,0.0f,0.0f);	/* Color Red	 */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Top Right Of The Quad (Front) */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Top Left Of The Quad (Front) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Bottom Left Of The Quad (Front) */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Bottom Right Of The Quad (Front) */
    glColor3f(1.0f,1.0f,0.0f);	/* Color Yellow */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Top Right Of The Quad (Back) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Top Left Of The Quad (Back) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Bottom Left Of The Quad (Back) */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Bottom Right Of The Quad (Back) */
    glColor3f(0.0f,0.0f,1.0f);	/* Color Blue */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Top Right Of The Quad (Left) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Top Left Of The Quad (Left) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Bottom Left Of The Quad (Left) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Bottom Right Of The Quad (Left) */
    glColor3f(1.0f,0.0f,1.0f);	/* Color Violet */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Top Right Of The Quad (Right) */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Top Left Of The Quad (Right) */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Bottom Left Of The Quad (Right) */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Bottom Right Of The Quad (Right) */
  glEnd();			/* End Drawing The Cube */


  glPolygonMode( GL_BACK, GL_LINE );
  glPolygonMode( GL_FRONT, GL_LINE );

  glBegin(GL_QUADS);		/* Draw The Cube Using quads */
    glColor3f(0.0f,0.0f,0.0f);	/* Color Blue */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Top Right Of The Quad (Top) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Top Left Of The Quad (Top) */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Bottom Left Of The Quad (Top) */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Bottom Right Of The Quad (Top) */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Top Right Of The Quad (Bottom) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Top Left Of The Quad (Bottom) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Bottom Left Of The Quad (Bottom) */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Bottom Right Of The Quad (Bottom) */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Top Right Of The Quad (Front) */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Top Left Of The Quad (Front) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Bottom Left Of The Quad (Front) */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Bottom Right Of The Quad (Front) */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Top Right Of The Quad (Back) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Top Left Of The Quad (Back) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Bottom Left Of The Quad (Back) */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Bottom Right Of The Quad (Back) */
    glVertex3f(-1.0f, 1.0f, 1.0f);	/* Top Right Of The Quad (Left) */
    glVertex3f(-1.0f, 1.0f,-1.0f);	/* Top Left Of The Quad (Left) */
    glVertex3f(-1.0f,-1.0f,-1.0f);	/* Bottom Left Of The Quad (Left) */
    glVertex3f(-1.0f,-1.0f, 1.0f);	/* Bottom Right Of The Quad (Left) */
    glVertex3f( 1.0f, 1.0f,-1.0f);	/* Top Right Of The Quad (Right) */
    glVertex3f( 1.0f, 1.0f, 1.0f);	/* Top Left Of The Quad (Right) */
    glVertex3f( 1.0f,-1.0f, 1.0f);	/* Bottom Left Of The Quad (Right) */
    glVertex3f( 1.0f,-1.0f,-1.0f);	/* Bottom Right Of The Quad (Right) */
  glEnd();			/* End Drawing The Cube */

  glPolygonMode( GL_BACK, GL_FILL );
  glPolygonMode( GL_FRONT, GL_FILL );

  glEndList();


  /* Generate Shadow */

  boxShadowDL = glGenLists(1);

  glNewList(boxShadowDL,GL_COMPILE);
  
  //glPolygonMode( GL_BACK, GL_FILL );
  //glPolygonMode( GL_FRONT, GL_POINT );
  glCullFace(GL_FRONT);

  glBegin(GL_QUADS);		/* Draw The Cube Using quads */
    glColor4f(0,0,0,0.5f);	/* Color Blue */
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
    
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    
    glVertex3f( 1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);
    
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
    
  glEnd();

  glCullFace(GL_BACK);

  //glPolygonMode( GL_BACK, GL_NONE );
  //glPolygonMode( GL_FRONT, GL_FILL );

  
  glEndList();

  return(boxDL);
}



void graphics::createFloor()
{
  GLuint image;		//This is our texture
  SDL_Surface *temp;	//This will help get the pixel data for our texture
 
  if ( (temp = SDL_LoadBMP("tile.bmp")) ) {
        
    //Check that the image is square (width equal to height)
    if (temp->w != temp->h) {
      return;
    }    
 
    //Check that the image's width is valid and then check that the image's width is a power of 2
    if (temp->w < 1) {
      return;
    } else if ( !((temp->w & (temp->w-1))==0) ) {
      return;
    }    
        
    //Create the texture
    glGenTextures(1, &image);
 
    //Load the texture
    glBindTexture(GL_TEXTURE_2D, image);
 
    //Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
 
    //Use nearest filtering, very good
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else {
    return;
  }    
 
  //Free up our temp surface if it was ever used
  if (temp) { SDL_FreeSurface(temp); }



  floorDL = glGenLists(1);

  glNewList(floorDL,GL_COMPILE);

  
  //Make certain everything is cleared from the screen before we draw to it
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  //Enable texturing
  glEnable(GL_TEXTURE_2D);
 
  //Load the texture
  glBindTexture(GL_TEXTURE_2D, image);
 
  glDisable( GL_CULL_FACE );
  glBegin(GL_QUADS);
  glColor3f( 1,1,1 );

  //Top-left vertex (corner)
  glTexCoord2i(0,0);
  glVertex3f(-200.0f, -0.05f, 200.0f);
	
  //Bottom-left vertex (corner)
  glTexCoord2i(60,0);
  glVertex3f(-200.0f, -0.05f, -200.0f);
	
  //Bottom-right vertex (corner)
  glTexCoord2i(60,60);
  glVertex3f(200.0f, -0.05f, -200.0f);
	
  //Top-right vertex (corner)
  glTexCoord2i(0,60);
  glVertex3f(200.0f, -0.05f, 200.0f);
  glEnd();

  glEnable( GL_CULL_FACE );
 
  //Disable texturing
  glDisable(GL_TEXTURE_2D);
  
  glEndList();


}





void graphics::toggleFullScreen(){
  SDL_WM_ToggleFullScreen( surface );
}






void graphics::drawBox()
{
  /*float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
  float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);*/

  glCallList(boxDL);
}


void graphics::moveCamera( float x, float y, float z ){
  cameraPos.x += (sin(cameraRot.y*PI/180)*z + cos(cameraRot.y*PI/180)*x);
  cameraPos.y += y;
  cameraPos.z += (-cos(cameraRot.y*PI/180)*z + sin(cameraRot.y*PI/180)*x);

}

void graphics::rotateCamera( float x, float y, float z ){
  cameraRot.x += x;
  cameraRot.y += y;
  cameraRot.z += z;
}


void graphics::setCamera( vector3 pos, vector3 rot ){
  cameraPos = pos;
  cameraRot = rot;
}




static void matrix_ODEtoGL(float* M, const dReal* p, const dReal* R)
{
    M[0]  = R[0]; M[1]  = R[4]; M[2]  = R[8];  M[3]  = 0;
    M[4]  = R[1]; M[5]  = R[5]; M[6]  = R[9];  M[7]  = 0;
    M[8]  = R[2]; M[9]  = R[6]; M[10] = R[10]; M[11] = 0;
    M[12] = p[0]; M[13] = p[1]; M[14] = p[2];  M[15] = 1;
}




void graphics::drawSubstratum( substratum* obj )
{
  //int type;
  dReal spos[3];
  const dReal* pos;
  const dReal* rot;



  obj->collide = false;

  if (!obj->geom)
    return;

  pos = dGeomGetPosition (obj->geom);

  rot = dGeomGetRotation (obj->geom);
  
  {
    dReal sides[3];
    float M[16];
    float M2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    dGeomBoxGetLengths(obj->geom, sides);

    glPushMatrix();
    
      matrix_ODEtoGL(M,pos, rot);
      glMultMatrixf(M);
    
      M2[0] = obj->size.x;
      M2[5] = obj->size.y;
      M2[10] = obj->size.z;
      M2[15] = 1;
      glMultMatrixf(M2);


      glCallList(boxDL);
    
    glPopMatrix();

    glPushMatrix();
      
      glScalef(1,0.01f,1);
      
      spos[0] = pos[0];
      spos[1] = 0;
      spos[2] = pos[2];
      matrix_ODEtoGL(M, (const dReal*)spos, rot);
      glMultMatrixf(M);
    
      M2[0] = obj->size.x;
      M2[5] = obj->size.y;
      M2[10] = obj->size.z;
      M2[15] = 1;
      glMultMatrixf(M2);

      glCallList(boxShadowDL);
    glPopMatrix();


  }
}





void graphics::drawScene( const vector< substratum *>* objs )
{
  int i;
  /* rotational vars for the triangle and quad, respectively */
  static GLfloat rtri, rquad;

  /* These are to calculate our fps */
  static GLint T0     = 0;
  static GLint Frames = 0;
  static float fps = 10;

  /* Clear The Screen And The Depth Buffer */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* Move Left 1.5 Units And Into The Screen 6.0 */
  glLoadIdentity();
  
  /* Rotate The Triangle On The Y axis ( NEW ) */
  glRotatef( cameraRot.x, 1,0,0 );
  glRotatef( cameraRot.y, 0,1,0 );
  glRotatef( cameraRot.z, 0,0,1 );

  glTranslatef( cameraPos.x, cameraPos.y, cameraPos.z );


  glCallList(floorDL);


  //glScalef( 1,5,1 );
  int numObjs = objs->size();
  for( i=0; i < numObjs; i++ ){
    drawSubstratum( (*objs)[i] );
  }

  /* Flush it to the screen */
  SDL_GL_SwapBuffers( );



  /* Gather our frames per second */
  Frames++;
  {
    GLint t = SDL_GetTicks();
    if (t - T0 >= 100) {
      GLfloat seconds = (t - T0) / 1000.0;
      fps = Frames / seconds;

      //printf("%d frames in %g seconds = %g FPS     %d\n", Frames, seconds, fps );

      T0 = t;
      Frames = 0;
    }


  }

 {
   GLint t = SDL_GetTicks();
   static GLint T0 = 0;
   int ms = (t-T0);

   if( ms < MIN_MSPF )
     SDL_Delay( MIN_MSPF-ms );

   T0 = t;
 }

  /* Increase The Rotation Variable For The Triangle ( NEW ) */
  rtri  += 0.2f;
  /* Decrease The Rotation Variable For The Quad     ( NEW ) */
  rquad -=0.15f;

}
