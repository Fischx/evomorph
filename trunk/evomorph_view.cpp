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

    creature->run( totalSteps );
    sim->step();
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
  m1 = new morf_node(vector3(1,2,1), 1, vector2(0.5,0.5),
		     6, vector2(0.5,0.5),
		     vector3(0,0,0), DENSITY, ann );

  //initializeAnn(&ann);
  m2 = new morf_node(vector3(1,0.5,0.5), 5, vector2(0.5,0.5),
		     3, vector2(0.5,0.5),
		     vector3(0,0,0), DENSITY, ann );
  m1->subnodes.push_back( m2 );


  //initializeAnn(&ann);
  m3 = new morf_node(vector3(1,0.5,0.5), 3, vector2(0.5,0.5),
		     4, vector2(0.5,0.5),
		     vector3(0,0,0), DENSITY, ann );

  m1->subnodes.push_back( m3 );
  return m1;
}
#endif


int main( int argc, char **argv )
{
  FILE * fd;
  morf_node* mnode;
  simulate * sim;
  gui* g;

  if( argc > 1 ){
    fd = fopen(argv[1], "r");
  }else{
    fd = fopen("population.evo", "r");
  }
  if( !fd ){
    printf("Could not open file: %s\n", argv[1] );
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
    body* phen;

    if( g->wantQuit )
      break;


    mnode = new morf_node();
    mnode->readTree(mnode, fd);

    phen = new body( sim->World, sim->Space, mnode );

    printf("Creature Fitness = %f \n", sim->getFitness( phen ) );

    delete phen;

    phen = new body( sim->World, sim->Space, mnode );
  
    show( sim, phen, g );
    delete phen;
    delete mnode;

  }
#endif
      
  fclose(fd);


  return 0;
}
