

#include "genotype.h"


morf_node::morf_node(){

}

morf_node::morf_node( morf_node* dad ){

  this->size = dad->size;
  this->size_variance = dad->size_variance;
  
  this->pos = dad->pos;
  this->pos_variance = dad->pos_variance;

  this->self_pos = dad->self_pos;
  this->self_pos_variance = dad->self_pos_variance;

  this->rot = dad->rot;
  this->rot_variance = dad->rot_variance;

  this->density = dad->density;
  this->density_variance = dad->density_variance;

  this->face = dad->face;
  this->self_face = dad->self_face;

  for( int j=0; j < NN_SIZE; j++ ){
    this->ann.weight[j] = dad->ann.weight[j];
    this->ann.variance[j] = dad->ann.variance[j];
  }

}


morf_node::morf_node( vector3 size, int face, vector2 pos,
		      int self_face, vector2 self_pos, 
		      vector3 rot, float density, ann_genotype ann){

  this->size = size;
  this->face = face;
  this->pos = pos;
  this->self_face = self_face;
  this->self_pos = self_pos;
  this->rot = rot;
  this->density = density;

  for( int j=0; j < NN_SIZE; j++ ){
    this->ann.weight[j] = ann.weight[j];
    this->ann.variance[j] = ann.variance[j];
  }

}

morf_node::~morf_node(){
  int size = subnodes.size();

  for( int i = 0; i < size; i++ ){
    if( subnodes[i] ){
      delete subnodes[i];
      subnodes[i] = NULL;
    }
  }

  subnodes.clear();
}

void morf_node::print_node(){
  printf("size: x = %f, y = %f, z = %f\n",this->size.x, this->size.y, this->size.z);
  printf("svar: x = %f, y = %f, z = %f\n",this->size_variance.x, this->size_variance.y, this->size_variance.z);
  printf("rot : x = %f, y = %f, z = %f\n",this->rot.x, this->rot.y, this->rot.z); 
  printf("rvar: x = %f, y = %f, z = %f\n",this->rot_variance.x, this->rot_variance.y, this->rot_variance.z); 
  printf("pos : x = %f, y = %f, face = %d\n",this->pos.x, this->pos.y, this->face); 
  printf("pvar: x = %f, y = %f\n",this->pos_variance.x, this->pos_variance.y); 
  printf("dens: %f\n",this->density);
  printf("dvar: %f\n",this->density_variance);
}




