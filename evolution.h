#ifndef _EVOLUTION_H_
#define _EVOLUTION_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "genotype.h"
#include "simulate.h"


//TODO: change TAU3 and TAU4 (NN_SIZE should be the number of nodes in a tree????)
#define TAU1     1/(sqrt(2 * sqrt(NN_SIZE)))
#define TAU2     1/(sqrt(2 * NN_SIZE))
#define TAU3     1/(sqrt(2 * sqrt(NN_SIZE)))
#define TAU4     1/(sqrt(2 * NN_SIZE))


#define N_POP    30   /* 1-generation population size */
#define BEST     10    /*  how many of the bests go to next gen at selectMix  */
#define WORST    0    /*  how many of the worsts go to next gen at selectMix */
/*  how many one should fight in selectLongPairwise    */
#define OPPONENTS (int) ceil(0.3 * N_POP)

#define SELECTION 2   /* {0,1,2} = {pairwise tournament, elit, mix}  */
                      /* note: mix = BEST bests, WORST worsts, and rest 
                         by pairwise tournament                         */


/* Non-Random initialization values */

#define DENSITY  1.0 /* box density */


/* Random initialization constraints  */
/* artificial neural network's */
#define WEIGHT_INIT_MAX   10.0
#define VARIANCE_INIT_MAX 1.0

/* box (or node) */
#define SIZE_INIT_MIN             0.5
#define SIZE_INIT_MAX             3.0
#define SIZE_VARIANCE_INIT_MAX    1.0
#define POS_INIT_MAX              0.9999
#define POS_VARIANCE_INIT_MAX     0.3
#define ROT_VARIANCE_INIT_MAX     0.0

#define DENSITY_INIT_MAX          10.0  /* not used yet, box density is fixed */
#define DENSITY_VARIANCE_INIT_MAX 0.5   /* not used yet, box density is fixed */

/* genotype */
#define MAXNODES 6
#define MINNODES 3

/* Mutation constraints */
#define SIZE_MIN  SIZE_INIT_MIN
#define SIZE_MAX  SIZE_INIT_MAX


/* Mutation chance ranges from 0 (never) to 1 (always) */
#define PROB_MUTATE_ANN              1.0
#define PROB_MUTATE_ANN_WEIGHT       0.99
#define PROB_MUTATE_BOX_SIZE         0.5
#define PROB_MUTATE_JUNCT_POS        0.5
#define PROB_MUTATE_JUNCT_POS_SELF   0.5
#define PROB_MUTATE_JUNCT_ROT        0.0

#define PROB_MUTATE_MORF_CUTANDPASTE 0.1
#define PROB_MUTATE_MORF_DELETE      0.01
#define PROB_MUTATE_MORF_CREATE      0.1


/* not recommended to mutate density!!! */
#define PROB_MUTATE_DENSITY         0.0



/*
 *  The whole population
 *  When applicable: [0..NPOP-1] == father, [NPOP..(2*NPOP)-1] == offspring
 */
typedef struct{
  morf_node* body_gen;
  double fitness;
  int victories;
  bool select;
} individual;


typedef morf_node* morf_tree;


class evolution
{
 public:
  void run();
  evolution();
  evolution(unsigned int seed);
  evolution(unsigned int seed, int selectionType);
  ~evolution();

 private:
  
  void initializeAnn(ann_genotype* ann);
  void mutateAnn( ann_genotype* father, ann_genotype* child );
  void mutateAnnRecursive(morf_tree tree, morf_tree result);

  void initializeNode(morf_node* node);
  void initializeBox(morf_node* node);

  morf_node* createRandomMorf();
  morf_node* createRandomMorfAux(int* numNodes);


  void mutateBox(const morf_node* father, morf_node* child );
  void mutateBoxRecursive(morf_node *tree, morf_node *result);
  void changeFace(vector2* pos, unsigned char* face);


  void mutateMorf_cutAndPaste(morf_tree tree);
  void mutateMorf_delete(morf_tree tree);
  void mutateMorf_create(morf_tree tree);

  /*
  void mutateMorf(morf_tree tree);
  void crossoverMorf(morf_tree dad, morf_tree mom, morf_tree son1, morf_tree son2);
  */



  static int compareFitness(const void *a, const void *b);
  static int compareVictories(const void *a, const void *b);


  void selectPairwise(  individual pop[N_POP*2] );
  void selectLongPairwise( individual spop[N_POP*2] );
  void selectElit( individual spop[N_POP*2]);
  void selectMix( individual spop[N_POP*2] );



  void getFitness( individual pop[N_POP*2] );

  void destroyInd( individual ind );
  void destroyPop( individual pop[N_POP*2] );

  void log(char *msg, char *file_name);
  void readConfig();

  individual pop[N_POP*2];

  int generation;
  int Selection;
  char plotlog[1024];
  simulate * sim;


};



#endif
