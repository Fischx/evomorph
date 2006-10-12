#include "substratum.h"

#include "simulate.h"

#include "genotype.h"
#include "body.h"
#include "evolution.h"


#include "gui.h"

#include <errno.h>
#include <stdio.h>

#include "global.h"


void show( simulate* sim, body* creature, gui* g ){
  int totalSteps = 0;
  g->show = true;


  g->setCamera((vector3(0.f,-15.f,-15.f) - creature->getPos()), vector3(45.f,0.f,0.f) );

  while( g->show && g->wantQuit == false ){

    g->setCreaturePos( creature->getPos() );

    g->handleEvents();

    g->drawScene( &creature->bodyParts );

    //#ifdef DEBUG
    if( g->slow ){
      //#endif
      creature->run( totalSteps );
      sim->step();
      //#ifdef DEBUG
    }
    //#endif
    totalSteps++;


  }

}




#ifdef DEBUG
morf_node* initDebugGen(){


  /* morf_node( vector3 size, int face, vector2 pos,
	     int self_face, vector2 self_pos, 
	     vector3 rot, float density, ann_genotype ann);
  */
  ann_genotype ann;
  morf_node *m1, *m2, *m3;

  //initializeAnn(&ann);
  m1 = new morf_node(vector3(1,1,1), 1, vector2(0.5,0.5),
		     6, vector2(0.5,0.5),
		     vector3(0,0,0), DENSITY, ann );

  //initializeAnn(&ann);
  m2 = new morf_node(vector3(1,0.5,1.5), 3, vector2(0.5,0.5),
		     6, vector2(0.5,0.5),
		     vector3(0*PI/6,-PI/4,PI/4), DENSITY, ann );
  m1->subnodes.push_back( m2 );


  //initializeAnn(&ann);
  m3 = new morf_node(vector3(2,0.5,0.5), 3, vector2(0.5,0.5),
		     5, vector2(0.5,0.5),
		     vector3(0,0,0), DENSITY, ann );

  m2->subnodes.push_back( m3 );
  return m1;
}
#endif


void printUsage( const char* prog ){
  printf("\nUsage: %s [-f POPULATION_FILE] [-m MIN_FITNESS]\n\n", prog );
  exit(-1);
}




int main( int argc, char **argv )
{
  FILE * fd;
  morf_node* mnode;
  simulate * sim;
  gui* g;
  int minFit = -12345;
  int go = -1;
  bool dontGetFit = false;
  fd = 0;
  for( int i=1; i<argc; i++ ){
    if( !strcmp(argv[i], "-m") ){
      if( i+1 >= argc )
	printUsage(argv[0]);
      minFit = atoi(argv[i+1]);
      i++;
    }

    if( !strcmp(argv[i], "-f") ){
      if( i+1 >= argc )
	printUsage(argv[0]);
      fd = fopen(argv[i+1], "r");
      if( !fd ){
	printf("Could not open file: %s\n", argv[i+1] );
	perror("");
	exit(-1);
      }
      i++;
    }


    if( !strcmp(argv[i], "-g") ){
      if( i+1 >= argc )
	printUsage(argv[0]);
      go = atoi(argv[i+1]);
      i++;
    }

    if( !strcmp(argv[i], "-n") ){
      dontGetFit = true;
    }


  }

  if( !fd )
    fd = fopen("population.evo", "r");
  if( !fd ){
    printf("Could not open file: %s\n", "population.evo" );
    perror("");
    exit(-1);
  }


  sim = new simulate();
  g = new gui();


#ifdef DEBUG

    body* phen;

    mnode = initDebugGen();

    phen = new body( sim->World, sim->Space, mnode );

    show( sim, phen, g );
    delete phen;
    delete mnode;

#else
  for( int i=0; i < N_POP; i++){
    float fit;
    body* phen;

    g->handleEvents();
    if( g->wantQuit )
      break;


    mnode = new morf_node();
    mnode->readTree(mnode, fd);

    if( go == -1 || i >= go ){
      phen = new body( sim->World, sim->Space, mnode );
      if( !dontGetFit )
	fit = sim->getFitness( phen );
      printf("Creature %d Fitness = %f \n", i, fit  );
      delete phen;
  
      if( minFit == -12345 || fit >= minFit ){
	phen = new body( sim->World, sim->Space, mnode );
	show( sim, phen, g );
	delete phen;
      }
    }

    delete mnode;

  }
#endif
      
  fclose(fd);


  return 0;
}
