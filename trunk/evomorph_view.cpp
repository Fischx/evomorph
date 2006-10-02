#include "substratum.h"

#include "simulate.h"

#include "genotype.h"
#include "body.h"
#include "evolution.h"


#include "gui.h"


void show( simulate* sim, body* creature, gui* g ){
  int totalSteps = 0;
  g->show = true;

  g->setCamera((vector3(0.f,-15.f,-15.f) - creature->getPos()), vector3(45.f,0.f,0.f) );

  while( g->show ){

    g->setCreaturePos( creature->getPos() );

    g->handleEvents();

    g->drawScene( &creature->bodyParts );

    creature->run( totalSteps );
    sim->step();
    totalSteps++;
  }

}


#include <errno.h>
#include <stdio.h>


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
  
  for( int i=0; i < N_POP; i++){
    body* phen;

    mnode = new morf_node();
    mnode->readTree(mnode, fd);

    phen = new body( sim->World, sim->Space, mnode );

    show( sim, phen, g );
    delete phen;
    delete mnode;

  }

      
  fclose(fd);


  return 0;
}
