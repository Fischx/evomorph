#include "substratum.h"

#include "simulate.h"

#include "genotype.h"
#include "body.h"
#include "evolution.h"


#include "gui.h"

int main( int argc, char **argv )
{
  FILE * fd;
  morf_node* mnode;
  simulate * sim;
  gui* g;
  sim = new simulate();
  g = new gui();

  fd = fopen("population.evo", "r");

  for( int i=0; i < N_POP; i++){
    body* phen;

    mnode = new morf_node();
    mnode->readTree(mnode, fd);

    phen = new body( sim->World, sim->Space, mnode );

    sim->show( phen, g );
    delete phen;
    delete mnode;

  }

      
  fclose(fd);


  return 0;
}
