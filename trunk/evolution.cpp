
#include "evolution.h"


/*
 *  Abramowitz and Stegun method for Normal random number
 *  Don't forget to set the seed BEFORE calling it for the
 *  first time (srand(seed);gaussrand())
 */
static double gaussrand()
{
  static double U, V;
  static int phase = 0;
  double Z;
    
  if(phase == 0) {
    U = (rand() + 1.) / (RAND_MAX + 2.);
    V = rand() / (RAND_MAX + 1.);
    Z = sqrt(-2 * log(U)) * sin(2 * M_PI * V);
  } 
  else
    Z = sqrt(-2 * log(U)) * cos(2 * M_PI * V);

  phase = 1 - phase;

  return Z;
}




evolution::evolution(){

  srand(time(NULL));
  Selection = SELECTION;
  readConfig();
  for(int i=0; i< N_POP*2; i++) {
    pop[i].body_gen = NULL;
  }
  sim = new simulate();

  for(int i=0; i< N_POP; i++) {
    pop[i].body_gen = createRandomMorf();
  }
}

evolution::evolution(unsigned int seed){

  srand(seed);
  Selection = SELECTION;
  readConfig();
  for(int i=0; i< N_POP*2; i++) {
    pop[i].body_gen = NULL;
  }
  sim = new simulate();
  for(int i=0; i< N_POP; i++) {
    pop[i].body_gen = createRandomMorf();
  }
}




evolution::evolution(unsigned int seed, int selectionType){

  srand(seed);
  Selection = selectionType;
  readConfig();
  for(int i=0; i< N_POP*2; i++) {
    pop[i].body_gen = NULL;
  }
  /*
  pop[0].body_gen = createRandomMorf();
  pop[0].body_gen->writeTree( pop[0].body_gen, NULL );
  //delete pop[0].body_gen;

  pop[1].body_gen = new morf_node();
  pop[1].body_gen->readTree( pop[1].body_gen, NULL );

  if( pop[1].body_gen->compareTree( pop[1].body_gen, pop[0].body_gen ) )
    printf("FFFFFFFFFFFFOOOOOOOOOOOOODDDDDDDDDDDEEEEEEEEEEEUUUUUUU\n");
  else
    printf("IGUALLLLLLLLL\n");
  exit(0);
  */
  sim = new simulate();
  for(int i=0; i< N_POP; i++) {
    pop[i].body_gen = createRandomMorf();
  }
}


evolution::~evolution(){
  destroyPop( pop );
  delete sim;

}


void evolution::destroyInd( individual ind ){
  if( ind.body_gen ){
    delete ind.body_gen;
    ind.body_gen = NULL;
  }
}

void evolution::destroyPop( individual dpop[N_POP*2] ){
  for(int i=0; i< N_POP*2; i++) {
    if( dpop[i].body_gen ){
      delete dpop[i].body_gen;
      dpop[i].body_gen = NULL;
    }
  }
}



/* 
 *  Initializes randomly an ann (varies from -?_INIT_MAX to +?_INIT_MAX) 
 */
void evolution::initializeAnn(ann_genotype* ann)
{
  int i;
  double isnegative; /* decides whether the generated random number is negative */
  for(i=0; i < NN_SIZE; i++) {
    ann->weight[i] = (WEIGHT_INIT_MAX*rand()/(RAND_MAX+1.0));
    ann->variance[i] = (VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
    /* Should the weight be negative? */
    isnegative = (rand()/(RAND_MAX+1.0));
    if(isnegative > 0.5) {
      ann->weight[i] = ann->weight[i] * -1;
    }
  }
}


/*
 *  Returns a single child mutated from the father
 *  Don't forget to set the seed before calling it
 */
void evolution::mutateAnn( ann_genotype* father, ann_genotype* child )
{
  int i;
  double normal;      /* interaction-independent normal random variable */
  double normal_i;    /* interaction-dependent normal random variable   */
  double probability; /* should the mutation (as a whole) happen? */
  double probability2;/* should the mutation (of each weight) happen? */


  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  /* Checks whether we should mutate this ann at all  */
  if(probability <= PROB_MUTATE_ANN) {
    normal = gaussrand();
    for(i=0; i < NN_SIZE; i++) {
      probability2 =  0 + (1.0*rand() / (RAND_MAX + 1.0));
      /* Should we mutate this weight? */
      if(probability2 <= PROB_MUTATE_ANN_WEIGHT) {      
        normal_i = gaussrand();
        child->variance[i] = father->variance[i] * exp( (TAU2 * normal) + (TAU1 * normal_i) );
        child->weight[i] = father->weight[i] + (child->variance[i] * normal_i);
      }
      /* No mutation for this weight */
      else {
        child->variance[i] = father->variance[i];
        child->weight[i] = father->weight[i];
      }
    }
  }
  /* No mutation for this ann */
  else {
    for(i=0; i < NN_SIZE; i++) {
      child->variance[i] = father->variance[i];
      child->weight[i] = father->weight[i];
    }
  }
}


/* 
 *  Initializes randomly an box node (except for its ann) (parameters varies from 0 or *_INIT_MAX)
 */
void evolution::initializeBox(morf_node* node)
{
  /* -----  size  ----- */
  node->size.x = SIZE_INIT_MIN + (SIZE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->size.y = SIZE_INIT_MIN + (SIZE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->size.z = SIZE_INIT_MIN + (SIZE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->size_variance.x = 0    + (SIZE_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->size_variance.y = 0    + (SIZE_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->size_variance.z = 0    + (SIZE_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));

  /* -----  father's junction position  ----- */
  node->face =  1          + (int)(6.0*rand()/(RAND_MAX+1.0));  /* a cube has 6 faces */
  
  node->pos.x = 0          + (POS_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->pos.y = 0          + (POS_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->pos_variance.x = 0 + (POS_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->pos_variance.y = 0 + (POS_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));

  /* -----  self junction position  ----- */
  node->self_face =  1          + (int)(6.0*rand()/(RAND_MAX+1.0));  /* a cube has 6 faces */
  
  node->self_pos.x = 0          + (POS_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->self_pos.y = 0          + (POS_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->self_pos_variance.x = 0 + (POS_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->self_pos_variance.y = 0 + (POS_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));

  /* -----  junction rotation  ----- */
  node->rot.x = 0;//          + (360.0*rand()/(RAND_MAX+1.0));     /* 360º */
  node->rot.y = 0;//          + (360.0*rand()/(RAND_MAX+1.0));
  node->rot.z = 0;//          + (360.0*rand()/(RAND_MAX+1.0));
  node->rot_variance.x = 0 + (ROT_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->rot_variance.y = 0 + (ROT_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->rot_variance.z = 0 + (ROT_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));

  /* -----  density  ----- */
  // mode0>density = 0          + (DENSITY_INIT_MAX*rand()/(RAND_MAX+1.0));
  node->density = DENSITY;
  node->density_variance = 0 + (DENSITY_VARIANCE_INIT_MAX*rand()/(RAND_MAX+1.0));
}

/* 
 *  Initializes randomly an box node (including for its ann) [wrapper only]
 */
void evolution::initializeNode(morf_node* node)
{
  initializeBox(node);
  initializeAnn(&node->ann);
}



#include "global.h"
morf_node* evolution::createRandomMorf(){

  int numNodes = MINNODES + (int)((MAXNODES-MINNODES+1) * (rand() / ((double)RAND_MAX + 1.0)));

  return createRandomMorfAux( &numNodes );
}

morf_node* evolution::createRandomMorfAux(int* numNodes ){
  int kids, k;
  morf_node* m;

  m = new morf_node();
  initializeNode(m);

  if( (*numNodes) <=0 )
    return m;

  kids = (((*numNodes)+1) * (rand()/((double)RAND_MAX+1.0)));

  if( (*numNodes) >= MINNODES  && kids == 0 )
    kids = 1;

  (*numNodes) -= kids;

  for(k=0; k < kids; k++){
    m->subnodes.push_back( createRandomMorfAux(numNodes) );
  }

  return m;
}



/*
 *  Auxiliary function to mutateBox
 */
void evolution::changeFace(vector2* pos, unsigned char* face)
{
  div_t res;
  float temp;
  
  /* 1,0 -> 1,99 == walk one face       */
  /* 2,0 -> 2,99 == walk two faces      */
  /* 3,0 -> 3,99 == walk three faces    */
  /* 4,0 -> 4,99 == it is the same face */
  while((pos->x >= 1) || (pos->x < 0) || (pos->y >= 1) || (pos->y < 0)) {
    /*  =============== x ===============  */
    if((pos->x >= 1) || (pos->x < 0)) {
      res = div((int)truncf(pos->x),4);
      pos->x = pos->x - (res.quot * 4);
      if (pos->x < 0) {
        pos->x = 4 + pos->x;
        res.rem = 3 - abs(res.rem);
      }
      pos->x = pos->x - truncf(pos->x);

      if(res.rem >=1){
        switch(face[0]){
        case 1:
          if (res.rem == 1) {
            face[0] = 3;
          }
          if (res.rem == 2) {
            face[0] = 6;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 5;
            pos->x = 1 - pos->x;
          }
          break;
        case 2:
          if (res.rem == 1) {
            face[0] = 3;
            temp = pos->x;
            pos->x = pos->y;
            pos->y = temp;
          }
          if (res.rem == 2) {
            face[0] = 4;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 5;
            temp = pos->x;
            pos->x = pos->y;
            pos->y = 1 - temp;
          }
          break;
        case 3:
          if (res.rem == 1) {
            face[0] = 6;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 2) {
            face[0] = 5;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 1;
          }
          break;
        case 4:
          if (res.rem == 1) {
            face[0] = 3;
            temp = pos->x;
            pos->x = pos->y;
            pos->y = 1 - temp;
          }
          if (res.rem == 2) {
            face[0] = 2;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 5;
            temp = pos->x;
            pos->x = pos->y;
            pos->y = temp;
          }
          break;
        case 5:
          if (res.rem == 1) {
            face[0] = 6;
          }
          if (res.rem == 2) {
            face[0] = 3;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 1;
            pos->x = 1 - pos->x;
          }
          break;
        case 6:
          if (res.rem == 1) {
            face[0] = 3;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 2) {
            face[0] = 1;
            pos->x = 1 - pos->x;
          }
          if (res.rem == 3) {
            face[0] = 5;
          }
          break;
        }
      }
    }


    /*  =============== y ===============  */
    if((pos->y >= 1) || (pos->y < 0)) {
      res = div((int)truncf(pos->y),4);
      pos->y = pos->y - (res.quot * 4);
      if (pos->y < 0) {
        pos->y = 4 + pos->y;
        res.rem = 3 - abs(res.rem);
      }
      pos->y = pos->y - truncf(pos->y);
      if(res.rem >=1){
        switch(face[0]){
        case 1:
          if (res.rem == 1) {
            face[0] = 4;
          }
          if (res.rem == 2) {
            face[0] = 6;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 2;
            pos->y = 1 - pos->y;
          }
          break;
        case 2:
          if (res.rem == 1) {
            face[0] = 6;
          }
          if (res.rem == 2) {
            face[0] = 4;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 1;
            pos->y = 1 - pos->y;
          }
          break;
        case 3:
          if (res.rem == 1) {
            face[0] = 4;
            temp = pos->y;
            pos->y = pos->x;
            pos->x = 1 - temp;
          }
          if (res.rem == 2) {
            face[0] = 5;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 2;
            temp = pos->y;
            pos->y = pos->x;
            pos->x = temp;
          }
          break;
        case 4:
          if (res.rem == 1) {
            face[0] = 6;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 2) {
            face[0] = 2;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 1;
          }
          break;
        case 5:
          if (res.rem == 1) {
            face[0] = 4;
            temp = pos->y;
            pos->y = pos->x;
            pos->x = temp;
          }
          if (res.rem == 2) {
            face[0] = 3;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 2;
            temp = pos->y;
            pos->y = pos->x;
            pos->x = 1 - temp;
          }
          break;
        case 6:
          if (res.rem == 1) {
            face[0] = 4;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 2) {
            face[0] = 1;
            pos->y = 1 - pos->y;
          }
          if (res.rem == 3) {
            face[0] = 2;
          }
          break;
        }
      }
    }
  }
}

void evolution::mutateBox(const morf_node* father, morf_node* child )
{
  double normal;      /* interaction-independent normal random variable */
  double normal_i;    /* interaction-dependent normal random variable   */
  double probability; /* should the mutation happen? */

  /* -----  size  ----- */
  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  if(probability <= PROB_MUTATE_BOX_SIZE) {
    //printf("mutando size...");
    normal = gaussrand();

    /* x */
    normal_i = gaussrand();
    child->size_variance.x = father->size_variance.x * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->size.x = father->size.x + (child->size_variance.x * normal_i);

    /* y */
    normal_i = gaussrand();
    child->size_variance.y = father->size_variance.y * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->size.y = father->size.y + (child->size_variance.y * normal_i);

    /* z */
    normal_i = gaussrand();
    child->size_variance.z = father->size_variance.z * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->size.z = father->size.z + (child->size_variance.z * normal_i);

    /* Constraints limiting */
    if(child->size.x > SIZE_MAX)
      child->size.x = SIZE_MAX;
    if(child->size.x < SIZE_MIN)
      child->size.x = SIZE_MIN;
    if(child->size.y > SIZE_MAX)
      child->size.y = SIZE_MAX;
    if(child->size.y < SIZE_MIN)
      child->size.y = SIZE_MIN;
    if(child->size.z > SIZE_MAX)
      child->size.z = SIZE_MAX;
    if(child->size.z < SIZE_MIN)
      child->size.z = SIZE_MIN;
    
    //printf("ok\n");
  }
  else {
    child->size = father->size;
    child->size_variance = father->size_variance;
  }

  /* -----  father's junction position  ----- */
  child->face = father->face; /* will be changed inside if necessary */
  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  if(probability <= PROB_MUTATE_JUNCT_POS) {
    //printf("mutando pos....");
    normal = gaussrand();

    /* x */
    normal_i = gaussrand();
    child->pos_variance.x = father->pos_variance.x * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->pos.x = father->pos.x + (child->pos_variance.x * normal_i);

    /* y */
    normal_i = gaussrand();
    child->pos_variance.y = father->pos_variance.y * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->pos.y = father->pos.y + (child->pos_variance.y * normal_i);

    /* do we need to change the face too? */
    changeFace(&child->pos, &child->face);

    //printf("ok\n");
  }
  else {
    child->pos = father->pos;
    child->pos_variance = father->pos_variance;
  }


  /* -----  self junction position  ----- */
  child->self_face = father->self_face; /* will be changed inside if necessary */
  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  if(probability <= PROB_MUTATE_JUNCT_POS_SELF) {
    //printf("mutando poss...");
    normal = gaussrand();

    /* x */
    normal_i = gaussrand();
    child->self_pos_variance.x = father->self_pos_variance.x * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->self_pos.x = father->self_pos.x + (child->self_pos_variance.x * normal_i);

    /* y */
    normal_i = gaussrand();
    child->self_pos_variance.y = father->self_pos_variance.y * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->self_pos.y = father->self_pos.y + (child->self_pos_variance.y * normal_i);

    /* do we need to change the face too? */
    changeFace(&child->self_pos, &child->self_face);

    //printf("ok\n");
  }
  else {
    child->self_pos = father->self_pos;
    child->self_pos_variance = father->self_pos_variance;
  }

  
  /* -----  junction rotation  ----- */
  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  if(probability <= PROB_MUTATE_JUNCT_ROT) {
    //printf("mutando rot....");
    normal = gaussrand();

    /* x */
    normal_i = gaussrand();
    child->rot_variance.x = father->rot_variance.x * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->rot.x = father->rot.x + (child->rot_variance.x * normal_i);

    /* y */
    normal_i = gaussrand();
    child->rot_variance.y = father->rot_variance.y * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->rot.y = father->rot.y + (child->rot_variance.y * normal_i);

    /* z */
    normal_i = gaussrand();
    child->rot_variance.z = father->rot_variance.z * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->rot.z = father->rot.z + (child->rot_variance.z * normal_i);
    
    //printf("ok\n");
  }
  else {
    child->rot = father->rot;
    child->rot_variance = father->rot_variance;
  }

  /* -----  density  ----- */
  probability =  0 + (1.0*rand() / (RAND_MAX + 1.0));
  if(probability <= PROB_MUTATE_DENSITY) {
    //printf("mutando dens...");
    normal = gaussrand();

    normal_i = gaussrand();
    child->density_variance = father->density_variance * exp( (TAU4 * normal) + (TAU3 * normal_i) );
    child->density = father->density + (child->density_variance * normal_i);

    //printf("ok\n");
  }
  else {
    child->density = father->density;
    child->density_variance = father->density_variance;
  }

}











void evolution::mutateBoxRecursive(morf_tree tree, morf_tree result) {
  int i;

  if (tree==NULL) return;

  mutateBox(tree, result);

  for(i=0;i<tree->subnodes.size();i++)
    mutateBoxRecursive(tree->subnodes[i],result->subnodes[i]);
}



void evolution::mutateAnnRecursive(morf_tree tree, morf_tree result) {
  int i;

  if (tree==NULL) return;


  mutateAnn( &tree->ann, &result->ann );

  for(i=0;i<tree->subnodes.size();i++)
    mutateAnnRecursive(tree->subnodes[i],result->subnodes[i]);
}



void evolution::mutateMorf_cutAndPaste(morf_tree tree) {
  int pos, size, son_index;
  morf_tree dad, son, dad2, son2;

  /* Checks whether we should mutate this Morf at all  */
  if(!((0 + (1.0*rand() / (RAND_MAX + 1.0))) <= PROB_MUTATE_MORF_CUTANDPASTE)) return;

  size = tree->numNodes();
  if( size <= 1 ) return;
  pos = 2 + (int)((size-1) * (double)rand()/(RAND_MAX+1.0));  


  tree->position(pos, &dad, &son, &son_index);

  dad->subnodes.erase( dad->subnodes.begin() + son_index ); //remove the tree Son from Dad

  size = tree->numNodes();
  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  

  if( pos == 1 ){
    tree->subnodes.push_back( son ); //insert the removed tree at another place
  }else{
    tree->position( pos, &dad2, &son2, &son_index);
    son2->subnodes.push_back( son ); //insert the removed tree at another place
  }
}

void evolution::mutateMorf_delete(morf_tree tree) {
  int pos, size, son_index;
  morf_tree dad, son;

  /* Checks whether we should mutate this Morf at all  */
  if(!((0 + (1.0*rand() / (RAND_MAX + 1.0))) <= PROB_MUTATE_MORF_DELETE)) return;


  size = tree->numNodes();
  if( size <= MINNODES ) return;
  pos = 2 + (int)((size-1) * (double)rand()/(RAND_MAX+1.0));  


  tree->position( pos, &dad, &son, &son_index);

  dad->subnodes.erase( dad->subnodes.begin() + son_index ); //remove the tree Son from Dad

  delete son;
}


void evolution::mutateMorf_create(morf_tree tree) {
  int pos, size, son_index;
  morf_tree created_morf, dad2, son2;

  /* Checks whether we should mutate this Morf at all  */
  if(!((0 + (1.0*rand() / (RAND_MAX + 1.0))) <= PROB_MUTATE_MORF_CREATE)) return;


  size = tree->numNodes();

  int numNodes = 1+(int)((MAXNODES-size) * (rand() / ((double)RAND_MAX + 1.0)));
  if( (numNodes+size) >= MAXNODES ) return;

  created_morf = createRandomMorfAux( &numNodes );


  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  
  if( pos == 1 ){
    tree->subnodes.push_back( created_morf ); //insert the removed tree at another place
  }else{

    tree->position(pos, &dad2, &son2, &son_index);
    son2->subnodes.push_back( created_morf ); //insert the removed tree at another place
  }
}





void evolution::mutateMorf_duplicate(morf_tree tree) {
  int pos, size, son_index;
  morf_tree duplicated_morf, dad, son, dad2, son2;

  /* Checks whether we should mutate this Morf at all  */
  if(!((0 + (1.0*rand() / (RAND_MAX + 1.0))) <= PROB_MUTATE_MORF_DUPLICATE)) return;


  size = tree->numNodes();
  if( size >= MAXNODES ) return;
  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  
  tree->position( pos, &dad, &son, &son_index);
  duplicated_morf = son->duplicate();


  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  
  tree->position(pos, &dad2, &son2, &son_index);
  son2->subnodes.push_back( duplicated_morf ); //insert the removed tree at another place
  
}




void evolution::mutateMorf_changeNN(morf_tree tree) {
  int pos, size, son_index;
  morf_tree duplicated_morf, dad, son, dad2, son2;

  /* Checks whether we should mutate this Morf at all  */
  if(!((0 + (1.0*rand() / (RAND_MAX + 1.0))) <= PROB_MUTATE_MORF_CHANGE_NN)) return;


  size = tree->numNodes();
  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  
  tree->position( pos, &dad, &son, &son_index);

  pos = 1 + (int)((size) * (double)rand()/(RAND_MAX+1.0));  
  tree->position(pos, &dad2, &son2, &son_index);
  
  for( int i=0; i < NN_SIZE; i++){
    son->ann.weight[i] = son2->ann.weight[i];
    son->ann.variance[i] = son2->ann.variance[i];
  }

}





/*
void evolution::crossoverMorf(morf_tree dad, morf_tree mom, morf_tree son1, morf_tree son2) {
  int num_dad=0,num_mom=0,leftNodes;
  morf_tree pos_dad, pos_mom, aux;

  numNodes(dad,&num_dad);
  leftNodes = (int)(num_dad * (double)rand() / (RAND_MAX+1.0));  
  position(dad,&pos_dad,&leftNodes);

  numNodes(mom,&num_mom);
  leftNodes = (int)(num_mom * (double)rand() / (RAND_MAX+1.0));  
  position(mom,&pos_mom,&leftNodes);

  aux = duplicateMorf(pos_dad);
  pos_dad = duplicateMorf(pos_mom);
  pos_mom = duplicateMorf(aux);

  numNodes(mom,&num_mom);
  if (num_mom > MAXNODES) pos_mom = duplicateMorf(pos_dad);

  numNodes(dad,&num_dad);
  if (num_dad > MAXNODES) pos_dad = duplicateMorf(aux);

}


*/
















/* Quicksort Comparison */
int evolution::compareFitness(const void *a, const void *b)
{
  if ((*(individual *) a).fitness < (*(individual*) b).fitness)
    return -1;
  if ((*(individual *) a).fitness == (*(individual *) b).fitness)
    return 0;
  if((*(individual *) a).fitness > (*(individual *) b).fitness)
    return 1;
  /* compiler anti-warning  */
  return 0;
}

/* Another quicksort Comparison */
int evolution::compareVictories(const void *a, const void *b)
{
  if ((*(individual *) a).victories < (*(individual*) b).victories)
    return -1;
  if ((*(individual *) a).victories == (*(individual *) b).victories)
    return 0;
  if((*(individual *) a).victories > (*(individual *) b).victories)
    return 1;
  /* compiler anti-warning  */
  return 0;
}




/*
 *  Pairwise tournament selection among fathers + offspring
 *  Returns the new population selected from the one in parameters
 */
void evolution::selectPairwise( individual spop[N_POP*2] )
{
  int i, k, l, s;
  individual newpop[N_POP];


  for(i=0; i< N_POP*2; i++){
    spop[i].select = false;
  }


  for(i=0; i< N_POP; i++) {

    do{
      /* two random  individuals */
      k = 0 + (int) ((2 * N_POP) * (rand() / (RAND_MAX + 1.0)));
      l = 0 + (int) ((2 * N_POP) * (rand() / (RAND_MAX + 1.0)));

      if (spop[k].fitness < spop[l].fitness) {
        s = l;
      }
      else {
        s = k;
      }
    }while( spop[s].select );

    spop[s].select = true;
    newpop[i] = spop[s];

  }

  for(i=0; i< N_POP*2; i++){
    if( !spop[i].select )
      destroyInd( spop[i] );
  }

  for(i=0; i< N_POP; i++){
    spop[i] = newpop[i];
  }
  for(i=N_POP; i< N_POP*2; i++){
    spop[i].body_gen = NULL;
  }

}

/*
 *  Long Pairwise tournament selection among fathers + offspring
 *  Each participant fights against OPONNENTS participants
 *  Returns the new population selected from best winners
 */
void evolution::selectLongPairwise( individual spop[N_POP*2] )
{
  int i, j, k;

  for(i=0; i< N_POP*2; i++){
    spop[i].select = false;
    spop[i].victories = 0;
  }

  /* Tournament! Each one fight against other OPPONENTS random individuals */
  for(i=0; i< N_POP*2; i++) {
    for(j=0; j < OPPONENTS; j++) {
      /* random individual */
      k = 0 + (int) ((2 * N_POP) * (rand() / (RAND_MAX + 1.0)));
      if(compareFitness((void *) &spop[i], (void *) &spop[k]) >= 0) {
        spop[i].victories++;
      }
    }
  }

  qsort(&spop[0], N_POP*2, sizeof(individual), compareVictories);
  
  for(i=0; i< N_POP; i++){
    destroyInd(spop[i]); // destroys the unused part
    spop[i] = spop [N_POP+i];
    spop [N_POP+i].body_gen = NULL;
  }
}


/* Selects ONLY the bests */
void evolution::selectElit( individual spop[N_POP*2] )
{
  int i;

  qsort(&spop[0], N_POP*2, sizeof(individual), compareFitness);

  for(i=0; i< N_POP; i++){
    destroyInd(spop[i]); // destroys the unused part
    spop[i] = spop [N_POP+i];
    spop [N_POP+i].body_gen = NULL;
  }

}

/* Selects BEST bests, WORST worsts and the rest by pairwise tournament */
void evolution::selectMix( individual spop[N_POP*2] )
{
  int i, k, l, s, pairwise;
  individual newpop[N_POP];

  pairwise = N_POP - BEST - WORST;
  if(pairwise < 0) {
    printf("BEST+WORSE > N_POP. Please readjust your parameters.\n");
    exit(1);
  }

  qsort(&spop[0], N_POP*2, sizeof(individual), compareFitness);
  for(i=0; i< N_POP*2; i++) {
    spop[i].select = false;
  }

  /*  bad things first */
  for(i=0; i< WORST; i++) {
    newpop[i] = spop[i];
    spop[i].select = true;
  }
  /* best things last */ /* makes qsort job easier! */
  for(i=0; i < BEST; i++) {
    newpop[N_POP-i-1] = spop[N_POP*2-i-1];
    spop[N_POP*2-i-1].select = true;
  }

  for(i=0; i< pairwise; i++) {

    do{
      /* two random  individuals */
      k = 0 + (int) ((2 * N_POP) * (rand() / (RAND_MAX + 1.0)));
      l = 0 + (int) ((2 * N_POP) * (rand() / (RAND_MAX + 1.0)));

      if (spop[k].fitness < spop[l].fitness) {
        s = l;
      }
      else {
        s = k;
      }
    }while( spop[s].select );
    spop[s].select = true;
    newpop[WORST+i] = spop[s];
  }

  for(i=0; i< N_POP*2; i++){
    if(!spop[i].select)
      destroyInd(spop[i]);
  }

  for(i=0; i< N_POP; i++){
    spop[i] = newpop[i];
  }

  for(i=N_POP; i< N_POP*2; i++){
    spop[i].body_gen = NULL;
  }

}



















void evolution::getFitness( individual fpop[N_POP*2] )
{
  body* phen;
  float meanFit = 0;
  float maxFit = 0;
  static float maxFitGlobal = 0;
  char msglog[1024];

  int j = 0;
  for( int i=0; i < N_POP*2; i++){

    phen = new body( sim->World, sim->Space, fpop[i].body_gen );

    static bool firstTime = true;
    if( i >= N_POP || firstTime )
      fpop[i].fitness = sim->getFitness( phen );
    if( i > N_POP )
      firstTime = false;

    meanFit += fpop[i].fitness;
    if(maxFit < fpop[i].fitness)
      maxFit = fpop[i].fitness;
    if(maxFitGlobal < fpop[i].fitness)
      maxFitGlobal = fpop[i].fitness;
    printf("Fitness = %f \n", fpop[i].fitness); fflush( stdout );

    delete phen;

    
    /*
    if( sim->evt->show && firstTime == false){
      body* phen;
      phen = new body( sim->World, sim->Space, fpop[N_POP-j-1].body_gen );
      printf( "Showing fitness = %f , popNumber = %d \n",fpop[N_POP-j-1].fitness,j );
      sim->show( phen );
      delete phen;
      j++;
      if( j >= N_POP ) j = 0;
    }
    */
  }

  meanFit = meanFit / (N_POP*2);
  printf( "------------------ gen %d mean fitness = %7.2f  max = %7.2f(%7.2f)\n", generation, meanFit, maxFit, maxFitGlobal);
  sprintf(msglog,"(%3d):meanfit=%7.2f, maxfit=%7.2f(%7.2f)\n",generation,meanFit,maxFit,maxFitGlobal);
  log(msglog,"logevolution.log");
  sprintf(msglog,"%d %f %f\n",generation,meanFit,maxFit);
  log(msglog, plotlog);
  generation++;
}






void evolution::log(char *msg, char *filename)
{
  FILE * fp;
  char complete_filename[1024];
  
  sprintf(complete_filename,"logs/%s",filename);    
  if ((fp= fopen(complete_filename, "a")) ==NULL)
    exit(1);
  fprintf(fp, "%s", msg);
  fclose(fp);
}

void evolution::readConfig()
{
    FILE * fp;
    char buffer[1024];
    int number;

    /* reading the file number */
    if ((fp= fopen("plotlog.config", "r+")) ==NULL)
        exit(1);
    fgets(buffer, 1023, fp);
    sprintf(plotlog,"plotlog_%s.log",buffer);
    printf("(%s)\n",plotlog);

    /* updating the file number */
    number = atoi(buffer);
    number++;
    fseek(fp, 0L, SEEK_SET);
    fprintf(fp,"%d", number);

    fclose(fp);
           
}  


void evolution::run(){
  int i;/*j;
          morf_node* m1, * m2, *m3;*/
  int dad, mom, son1, son2;
  double fit, lowerfit;
  char buf[64], msglog[1024];
  time_t now;

  time(&now);
  ctime_r(&now, buf);
  buf[strlen(buf)-1] = 0;
  sprintf(msglog,"\n======================================\n%s:\n",buf);
  log(msglog, "logevolution.log");
  sprintf(msglog,"Parameters:\nnpop:%d, selection:%d, best:%d, worst:%d, dens:%.2f\n\
init:  weight:%.2f(%.2f) size:%.2f-%.2f(%.2f), pos:%.2f(%.2f),rot:(%.2f)\n\
probs: ann: %.2f annw:%.2f size:%.2f pos:%.2f poss:%.2f rot:%.2f dens:%.2f\n\
gnuplot logging at %s\n",
          N_POP, Selection, BEST, WORST, DENSITY, WEIGHT_INIT_MAX, VARIANCE_INIT_MAX, SIZE_INIT_MIN, SIZE_INIT_MAX, SIZE_VARIANCE_INIT_MAX, 
          POS_INIT_MAX, POS_VARIANCE_INIT_MAX, ROT_VARIANCE_INIT_MAX, PROB_MUTATE_ANN, PROB_MUTATE_ANN_WEIGHT, 
          PROB_MUTATE_BOX_SIZE, PROB_MUTATE_JUNCT_POS, PROB_MUTATE_JUNCT_POS_SELF, PROB_MUTATE_JUNCT_ROT, PROB_MUTATE_DENSITY,plotlog);
  log(msglog,"logevolution.log");

  
  generation = 0;
  // do it:
  
  while( 1 ){
  
    for(i=0; i< N_POP; i++){

      pop[i+N_POP].body_gen = pop[i].body_gen->duplicate();

      // Apply mutation operators
      mutateBoxRecursive(pop[i].body_gen, pop[i+N_POP].body_gen);
      mutateAnnRecursive(pop[i].body_gen, pop[i+N_POP].body_gen);
      mutateMorf_cutAndPaste(pop[i+N_POP].body_gen);      
      mutateMorf_delete(pop[i+N_POP].body_gen);
      mutateMorf_create(pop[i+N_POP].body_gen);
      mutateMorf_duplicate(pop[i+N_POP].body_gen);
      mutateMorf_changeNN(pop[i+N_POP].body_gen);

    }

    getFitness( pop );


    if (Selection == 0)
      selectPairwise(pop);
    else if (Selection == 1)
      selectElit(pop);
    else if (Selection == 2)
      selectMix(pop);
    else if (Selection == 3)
      selectLongPairwise(pop);
    
    {
      FILE * fd;

      fd = fopen("population.evo", "w+");

      for( i=0; i < N_POP; i++ )
	pop[i].body_gen->writeTree(pop[i].body_gen, fd);
      
      fclose(fd);
    }
  }

  
}

