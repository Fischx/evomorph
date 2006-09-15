#include <math.h>
#include <ode/ode.h>


#include "substratum.h"
#include "vector3.h"



substratum::substratum( dWorldID world, dSpaceID space, vector3 size, vector3 pos, vector3 rot, float density, double ann_genotype[NN_SIZE]  )
{
  rateFactor1 = 1;
  rateFactor2 = 1;
  force1 = force2 = 0;

  dMatrix3 A, B, C, I, Rx, Ry, Rz;
  dRFromAxisAndAngle(Rx, 1, 0, 0, rot.x);
  dRFromAxisAndAngle(Ry, 0, 1, 0, rot.y);
  dRFromAxisAndAngle(Rz, 0, 0, 1, rot.z);

  dRFromAxisAndAngle(I, 1, 0, 0, 0);

  dMultiply0( A, I, Rx, 3, 3, 3 );
  dMultiply0( B, A, Ry, 3, 3, 3 );  
  dMultiply0( C, B, Rz, 3, 3, 3 );

  dBodySetRotation(this->body, C);

  substratum( world, space, size, pos, C, density, ann_genotype );
}


substratum::substratum( dWorldID world, dSpaceID space, vector3 size, vector3 pos, dMatrix3 rot, float density, double ann_genotype[NN_SIZE]  )
{
  rateFactor1 = 1;
  rateFactor2 = 1;
  force1 = force2 = 0;

  this->World = world;
  this->body = dBodyCreate(world);

  this->size = size;
  this->volume = (size.x*size.y*size.z);
  this->dad = NULL;
  
  dBodySetPosition(this->body, pos.x, pos.y, pos.z);
  
  dBodySetLinearVel( this->body, 0, 0, 0);

  dBodySetRotation(this->body, rot);

  dBodySetData( this->body, (void*)(this));
  
  dMass m;
  dReal sides[3];
  sides[0] = 2*size.x;
  sides[1] = 2*size.y;
  sides[2] = 2*size.z;
  dMassSetBox(&m, density, sides[0], sides[1], sides[2]);
  dBodySetMass(this->body, &m);

  this->geom = dCreateBox( space, sides[0], sides[1], sides[2]);
  dGeomSetBody( this->geom, this->body );


  collide = false;
  hasJoint = false;

  nn = new cNeuralNet( ann_genotype );

}

substratum::~substratum( )
{
  dGeomDestroy( this->geom );
  dBodyDestroy( this->body );
  if( hasJoint )
    dJointDestroy( this->jid );

  delete nn;
}




vector3 substratum::getPos(){
  const dReal* pos1;
  pos1 = dGeomGetPosition (this->geom);

  return vector3(pos1[0],pos1[1],pos1[2]);
}

void substratum::setPos( vector3 pos ){
  dBodySetPosition(this->body, pos.x, pos.y, pos.z);
}

void substratum::attach( const substratum* obj2)
{
  /*
  jid = dJointCreateHinge (World,0);

  dJointAttach (jid,obj1->body, obj2->body);

  dJointSetHingeAnchor (jid, 0, 0.5, 0);
 
  dJointSetHingeAxis (jid,1,0,0);

  dJointSetHingeParam(jid, dParamLoStop, -PI/1.5);

  dJointSetHingeParam(jid, dParamHiStop, PI/1.5); 
  */



  hasJoint = true;

  jid = dJointCreateUniversal(World,0);
  
  dJointAttach(jid, this->body, obj2->body);

  const dReal* pos1, * pos2;
  pos1 = dGeomGetPosition (this->geom);
  pos2 = dGeomGetPosition (obj2->geom);

  dJointSetUniversalAnchor ( jid, (pos1[0]+pos2[0])/2, (pos1[1]+pos2[1])/2, (pos1[2]+pos2[2])/2 );
  dJointSetUniversalAxis1 ( jid, 1, 0, 0);
  dJointSetUniversalAxis2 ( jid, 0, 1, 0);
 

  this->dad = (substratum*)obj2;
}


#include "global.h"

void substratum::attach( const substratum* obj2, vector3 attachpos, vector3 axis1, vector3 axis2 )
{

  hasJoint = true;

  jid = dJointCreateUniversal(World,0);
  
  dJointAttach(jid, this->body, obj2->body);

  
  dJointSetUniversalAnchor ( jid, attachpos.x, attachpos.y, attachpos.z);
  dJointSetUniversalAxis1 ( jid, axis1.x, axis1.y, axis1.z);
  dJointSetUniversalAxis2 ( jid, axis2.x, axis2.y, axis2.z);

  
  dJointSetUniversalParam ( jid, dParamLoStop, -PI/3);
  dJointSetUniversalParam ( jid, dParamHiStop, PI/3);
  dJointSetUniversalParam ( jid, dParamLoStop2, -PI/3);
  dJointSetUniversalParam ( jid, dParamHiStop2, PI/3);
  

  dJointSetUniversalParam ( jid, dParamCFM, 0.01f); 
  dJointSetUniversalParam ( jid, dParamStopERP, 0.7); 
  dJointSetUniversalParam ( jid, dParamStopCFM, 0.00001f );
  dJointSetUniversalParam ( jid, dParamSuspensionERP, 0.7);
  dJointSetUniversalParam ( jid, dParamSuspensionCFM, 0.00001f );
  
  dJointSetUniversalParam ( jid, dParamCFM2, 0.01f); 
  dJointSetUniversalParam ( jid, dParamStopERP2, 0.7); 
  dJointSetUniversalParam ( jid, dParamStopCFM2, 0.00001f);
  dJointSetUniversalParam ( jid, dParamSuspensionERP2, 0.7);
  dJointSetUniversalParam ( jid, dParamSuspensionCFM2, 0.00001f );
  /*
  dJointSetUniversalParam ( jid, dParamCFM, 0); 
  dJointSetUniversalParam ( jid, dParamStopERP, 1); 
  dJointSetUniversalParam ( jid, dParamStopCFM, 0);
  dJointSetUniversalParam ( jid, dParamSuspensionERP, 1);
  dJointSetUniversalParam ( jid, dParamSuspensionCFM, 0);
  */

  this->dad = (substratum*)obj2;
}


void substratum::getRateFactor( float rateFactor[2] ){
  
  float rate1, rate2;
  rate1 = dJointGetUniversalAngle1Rate( this->jid );
  rate2 = dJointGetUniversalAngle2Rate( this->jid );

  if( rate1 > MAX_ANGLE_RATE ){
    rateFactor1 /= 2;
    if( rateFactor1 < 0.1 ) rateFactor1 = 0;
  }else{
    if( rateFactor1 < 1 )
	rateFactor1 += RATE_INCREASE;
  }
  if( rateFactor1 > 1 )
    rateFactor1 = 1;
  rateFactor[0] = rateFactor1;

  if( rate2 > MAX_ANGLE_RATE ){
    rateFactor2 /= 2;
    if( rateFactor2 < 0.1 ) rateFactor2 = 0;
  }else{
    if( rateFactor2 < 1 )
	rateFactor2 += RATE_INCREASE;
  }
  if( rateFactor2 > 1 )
    rateFactor2 = 1;
  rateFactor[1] = rateFactor2;
}

