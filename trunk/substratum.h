#ifndef _SUBSTRATUM_H_
#define _SUBSTRATUM_H_



#include <ode/ode.h>

#include "vector3.h"
#include "neuralnet.h"

#define MAX_ANGLE_RATE 5
#define RATE_INCREASE 0.00001f

class substratum
{

 public:

  substratum( dWorldID world, dSpaceID space, vector3 size, vector3 pos, vector3 rot, float density, double ann_genotype[NN_SIZE] );
  substratum( dWorldID world, dSpaceID space, vector3 size, vector3 pos, dMatrix3 rot, float density, double ann_genotype[NN_SIZE]  );

  ~substratum();

  void attach( const substratum* obj2);
  void attach( const substratum* obj2, vector3 attachpos, vector3 axis1, vector3 axis2 );

  vector3 getPos();
  void setPos( vector3 pos );

  void getRateFactor( float rateFactor[2] );

  bool collide_floor;
  bool collide_body;


  dBodyID body;  /* the dynamics body */
  dGeomID geom;  /* geometrie representing this body */
  vector3 size;
  float volume;
  dJointID jid;

  bool hasJoint;
  cNeuralNet* nn;


  substratum* dad;



  float force1;
  float force2;
 private: 
  dWorldID World;

  float rateFactor1;
  float rateFactor2;



};

#endif
