

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



void morf_node::writeNode( morf_node* node, FILE* fd )
{

  fwrite( node->ann.weight, sizeof(double), NN_SIZE, fd);
  fwrite( node->ann.variance, sizeof(double), NN_SIZE, fd);

  fwrite( &node->size, sizeof(vector3), 1, fd);
  fwrite( &node->size_variance, sizeof(vector3), 1, fd);

  fwrite( &node->pos, sizeof(vector2), 1, fd);
  fwrite( &node->pos_variance, sizeof(vector2), 1, fd);

  fwrite( &node->self_pos, sizeof(vector2), 1, fd);
  fwrite( &node->self_pos_variance, sizeof(vector2), 1, fd);

  fwrite( &node->rot, sizeof(vector3), 1, fd);
  fwrite( &node->rot_variance, sizeof(vector3), 1, fd);

  fwrite( &node->density, sizeof(double), 1, fd);
  fwrite( &node->density_variance, sizeof(double), 1, fd);

  fwrite( &node->face, sizeof(unsigned char), 1, fd);
  fwrite( &node->self_face, sizeof(unsigned char), 1, fd);

}

void morf_node::writeTree(morf_node* tree, FILE* fd) 
{
  int i, size;

  if (tree==NULL) return;

  writeNode( tree, fd );
  size = tree->subnodes.size();
  fwrite( &size, sizeof(int), 1, fd);

  for (i=0;i<size;i++)
    writeTree(tree->subnodes[i], fd);
}



void morf_node::writeTree(morf_node* tree, const char* fileName ) 
{
  FILE * fd;

  if( fileName == NULL )
    fd = fopen("underEvolution.evo", "w+");
  else
    fd = fopen(fileName, "w+");

  writeTree(tree, fd);

  fclose(fd);
}







void morf_node::readNode( morf_node* node, FILE* fd )
{

  fread( node->ann.weight, sizeof(double), NN_SIZE, fd);
  fread( node->ann.variance, sizeof(double), NN_SIZE, fd);

  fread( &node->size, sizeof(vector3), 1, fd);
  fread( &node->size_variance, sizeof(vector3), 1, fd);

  fread( &node->pos, sizeof(vector2), 1, fd);
  fread( &node->pos_variance, sizeof(vector2), 1, fd);

  fread( &node->self_pos, sizeof(vector2), 1, fd);
  fread( &node->self_pos_variance, sizeof(vector2), 1, fd);

  fread( &node->rot, sizeof(vector3), 1, fd);
  fread( &node->rot_variance, sizeof(vector3), 1, fd);

  fread( &node->density, sizeof(double), 1, fd);
  fread( &node->density_variance, sizeof(double), 1, fd);

  fread( &node->face, sizeof(unsigned char), 1, fd);
  fread( &node->self_face, sizeof(unsigned char), 1, fd);

}




void morf_node::readTree(morf_node* tree, FILE* fd) 
{
  int i, size;

  if (tree==NULL) return;

  readNode( tree, fd );

  fread( &size, sizeof(int), 1, fd);

  for (i=0;i<size;i++){
    tree->subnodes.push_back( new morf_node() );

    readTree(tree->subnodes[i], fd);
  }
}


void morf_node::readTree(morf_node* tree, const char* fileName ) 
{
  FILE * fd;

  if( fileName == NULL )
    fd = fopen("underEvolution.evo", "r");
  else
    fd = fopen(fileName, "r");

  readTree(tree, fd);

  fclose(fd);
}






/* compare two nodes: */
/* return 0 to equal, 1 to different */
int morf_node::compareNode( morf_node* a, morf_node* b )
{
  int i;
  for( i=0; i < NN_SIZE; i++)
    if( (a->ann.weight[i] != b->ann.weight[i]) ||
	(a->ann.variance[i] != b->ann.variance[i]) )
      return 1;


  if( a->size != b->size ) return 1;
  if( a->size_variance != b->size_variance ) return 1;

  if( a->pos != b->pos ) return 1;
  if( a->pos_variance != b->pos_variance ) return 1;

  if( a->self_pos != b->self_pos ) return 1;
  if( a->self_pos_variance != b->self_pos_variance ) return 1;

  if( a->rot != b->rot ) return 1;
  if( a->rot_variance != b->rot_variance ) return 1;

  if( a->density != b->density ) return 1;
  if( a->density_variance != b->density_variance ) return 1;

  if( a->face != b->face ) return 1;
  if( a->self_face != b->self_face ) return 1;


  return 0;
}


int morf_node::compareTree(morf_node* a, morf_node* b) 
{
  int i;

  if (a==NULL && b == NULL) return 0;
  if (b==NULL && a != NULL) return 1;
  if (a==NULL && b != NULL) return 1;

  compareNode( a, b );

  if( a->subnodes.size() != b->subnodes.size() ) return 1;

  for (i=0;i<a->subnodes.size();i++){
    if( compareTree(a->subnodes[i], b->subnodes[i]) != 0 )
      return 1;
  }
  return 0;
}


int morf_node::numNodes() {
  int i, num;

  num = 1;

  for (i=0;i< this->subnodes.size();i++)
    num += this->subnodes[i]->numNodes();

  return num;
}


// This function returns the node at position pos.
// *p must be initialized with zero.
// It returns the node Son at position Pos and his Dad.
// The position count begins in 1 (one) and is like a deep-search in the tree.
// The root-node is the number 1
void morf_node::positionAux( int pos, int* p, morf_node** dad, morf_node** son, int* son_index) {
  int i;

  if ( (*p) >= pos ) return;

  if (pos <= 1 ){// must never reach this case, 
                 // because the One node is the root, so it has no dad

    (*dad) = this;
    (*son) = this;
    (*son_index) = 0;
  }

  (*p)++;
  (*son) = this;

  if( *p == pos ) return;

  for (i=0;i<this->subnodes.size();i++){
    (*dad) = this;
    (*son_index) = i;
    this->subnodes[i]->positionAux(pos,p,dad,son,son_index);
    if(*p >= pos) break;
  }
}

void morf_node::position( int pos, morf_node** dad, morf_node** son, int* son_index) {
  int p = 0;
  positionAux( pos, &p, dad, son, son_index);

}


morf_node* morf_node::duplicate(){
  morf_node* mnode, *sub;
  
  mnode = new morf_node( this );

  int size = this->subnodes.size();
  for( int i = 0; i < size; i ++ ){

    sub = this->subnodes[i]->duplicate();

    mnode->subnodes.push_back( sub );
  }

  return mnode;
}
