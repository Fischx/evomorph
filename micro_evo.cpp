#include "graphics.h"
#include "events.h"
#include "substratum.h"

#include "simulate.h"

#include "genotype.h"
#include "body.h"


#include "evolution.h"

void InvalidEntry() {
  printf("Usage: micro seed selection\n");
  printf("Where seed and selection are (optionals) integers\n");
  printf("If no seed, seed = time(NULL)\n");
  printf("Selection values: 0 (pairwise), 1 (elitist), 2 (mixed), 3 (longPairwise)\n");
  printf("If no selection, all is run\n");
  exit(1);
}

int main( int argc, char **argv )
{
  evolution* enn;
  /* parâmetro seed */
  char *param1, *param2;
  int seed,selectionType;
  unsigned int i;

  /* param1 = seed */
  if(argc > 1) {
    param1 = argv[1];
    for(i=0; i<strlen(param1); i++) {
      if(!isdigit(param1[i])) {
        InvalidEntry();
      }
    }
    seed = atoi(param1);
  }
  else
    seed = time(NULL);

  /* param2 = selection type */
  if(argc > 2) {
    param2 = argv[2];
    if(! ((strcmp(param2,"0") == 0) || (strcmp(param2,"1") == 0) || (strcmp(param2,"2") == 0) || (strcmp(param2,"3") == 0)) ) {
      InvalidEntry();
    }
    selectionType = atoi(param2);
  }
  else
    selectionType = SELECTION;
  

  enn = new evolution(seed, selectionType);

  enn->run();


  delete enn;

  exit(0);


}
