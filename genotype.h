#ifndef _GENOTYPE_H_
#define _GENOTYPE_H_

#include "vector3.h"
#include "vector2.h"

#include <vector>

using namespace std;


/*
 *  A single ANN (Artificial Neural Network), considering its genotype
 *  Composed by weights and their variance probability
 */
#include "neuralnet.h"
typedef struct{
  double weight[NN_SIZE];
  double variance[NN_SIZE];
} ann_genotype;




class morf_node{

 public:
  morf_node( morf_node* dad );

  morf_node( vector3 size, int face, vector2 pos,
	     int self_face, vector2 self_pos, 
	     vector3 rot, float density, ann_genotype ann);



  ~morf_node();
  morf_node();



 private:
  void print_node();

 public:

  ann_genotype ann;
  vector < morf_node* > subnodes;


  /* box size 3D */
  vector3 size;          /* x,y,z (0..max) */
  vector3 size_variance;


  /* junction position at father's box */
  vector2 pos;           /* x,y (0..0.9999) */
  vector2 pos_variance;


  /* junction position at itself's box */
  vector2 self_pos;           /* x,y (0..0.9999) */
  vector2 self_pos_variance;


  vector3 rot;
  vector3 rot_variance;

  float density;
  float density_variance;


  /* in which face is the junction (father's and self) */
  unsigned char face;
  unsigned char self_face;


};







#endif
