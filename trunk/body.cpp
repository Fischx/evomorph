#include "body.h"

#include "global.h"

#define SPC 0.5

substratum* body::create( morf_node* mnode, vector3 p_pos, vector3 p_size, vector3 p_rot, dMatrix3 p_MatRot, vector3* attach_pos, vector3* axis1, vector3* axis2 ){
  substratum* obj_p;
  substratum* obj_c;

  vector3 l_pos, l_rot;
  vector3 pos_m, att_m;

  switch( mnode->face ){
  case 1 : 
    att_m = vector3( 
		     ( 2*p_size.x * (mnode->pos.x-0.5)), 
		     -p_size.y - SPC/2,
		     -( 2*p_size.z * (mnode->pos.y-0.5))
		     );

    pos_m = att_m + 
      vector3( ( 2*mnode->size.x * (mnode->self_pos.x-0.5)),
	       -mnode->size.y - SPC/2 , 
	       -( 2*mnode->size.z * (mnode->self_pos.y-0.5))
	       );

    (*axis1) = vector3( 0,0,1);
    (*axis2) = vector3( -1,0,0);

    break;
  case 6 : 
    att_m = vector3( 
		     ( 2*p_size.x * (mnode->pos.x-0.5)), 
		     p_size.y + SPC/2,
		     -( 2*p_size.z * (mnode->pos.y-0.5))
		     );

    pos_m = att_m + 
      vector3( ( 2*mnode->size.x * (mnode->self_pos.x-0.5)),
	       mnode->size.y + SPC/2 , 
	       -( 2*mnode->size.z * (mnode->self_pos.y-0.5))
	       );


    (*axis1) = vector3( 0,0,1);
    (*axis2) = vector3( 1,0,0);

    break;
  case 3 :
    att_m = vector3( p_size.x + SPC/2,
		     ( 2*p_size.y * (mnode->pos.y-0.5)), 
		     -( 2*p_size.z * (mnode->pos.x-0.5))
		     );

    pos_m = att_m + 
      vector3( mnode->size.x + SPC/2 , 
	       ( 2*mnode->size.y * (mnode->self_pos.y-0.5)),
	       -( 2*mnode->size.z * (mnode->self_pos.x-0.5))
	       );


    (*axis1) = vector3( 0,0,1);
    (*axis2) = vector3( 0,-1,0);

    break;
  case 5 :
    att_m = vector3( -p_size.x - SPC/2,
		     ( 2*p_size.y * (mnode->pos.y-0.5)), 
		     -( 2*p_size.z * (mnode->pos.x-0.5))
		     );

    pos_m = att_m + 
      vector3( -mnode->size.x - SPC/2 , 
	       ( 2*mnode->size.y * (mnode->self_pos.y-0.5)),
	       -( 2*mnode->size.z * (mnode->self_pos.x-0.5))
	       );

    (*axis1) = vector3( 0,0,1);
    (*axis2) = vector3( 0,1,0);


    break;
  case 2 : 
    att_m = vector3( ( 2*p_size.x * (mnode->pos.x-0.5)), 
		     ( 2*p_size.y * (mnode->pos.y-0.5)),
		     p_size.z + SPC/2 
		     );

    pos_m = att_m + 
      vector3( ( 2*mnode->size.y * (mnode->self_pos.x-0.5)),
	       ( 2*mnode->size.z * (mnode->self_pos.y-0.5)),
	       mnode->size.z + SPC/2 
	       );


    (*axis1) = vector3( 1,0,0);
    (*axis2) = vector3( 0,1,0);


    break;
  case 4 :
    att_m = vector3( ( 2*p_size.x * (mnode->pos.x-0.5)), 
		     ( 2*p_size.y * (mnode->pos.y-0.5)),
		     -p_size.z - SPC/2 
		     );

    pos_m = att_m + 
      vector3( ( 2*mnode->size.y * (mnode->self_pos.x-0.5)),
	       ( 2*mnode->size.z * (mnode->self_pos.y-0.5)),
	       -mnode->size.z - SPC/2 
	       );


    (*axis1) = vector3( -1,0,0);
    (*axis2) = vector3( 0,1,0);

    break;
  }

  pos_m = pos_m.rotate( vector3(1,0,0), p_rot.x );
  pos_m = pos_m.rotate( vector3(0,1,0), p_rot.y );
  pos_m = pos_m.rotate( vector3(0,0,1), p_rot.z );

  att_m = att_m.rotate( vector3(1,0,0), p_rot.x );
  att_m = att_m.rotate( vector3(0,1,0), p_rot.y );
  att_m = att_m.rotate( vector3(0,0,1), p_rot.z );

  l_pos = p_pos + pos_m;
  (*attach_pos) = p_pos + att_m;

  l_rot = mnode->rot + p_rot;

  //-- Calc Rotation Matrix
    dMatrix3 Rx, Ry, Rz, A, B, C, I;

    dRSetIdentity( I );

    dRFromAxisAndAngle(Rx, 1, 0, 0, mnode->rot.x);
    dRFromAxisAndAngle(Ry, 0, 1, 0, mnode->rot.y);
    dRFromAxisAndAngle(Rz, 0, 0, 1, mnode->rot.z);

    dMultiply0( A, p_MatRot, Rx, 3, 3, 3 );
    dMultiply0( B, A, Ry, 3, 3, 3 );  
    dMultiply0( C, B, Rz, 3, 3, 3 );
  //--


  obj_p = new substratum( World, Space, mnode->size, l_pos, C, mnode->density, mnode->ann.weight );

  bodyParts.push_back( obj_p );


  //-- Calculate YPOS
    float yPos = 0;
    vector3 rsize = mnode->size + vector3(2*SPC,2*SPC,2*SPC);
    rsize = rsize.rotate( vector3(0,0,1), p_rot.z );
    rsize = rsize.rotate( vector3(0,1,0), p_rot.y );
    rsize = rsize.rotate( vector3(1,0,0), p_rot.x );
    yPos = l_pos.y - rsize.y;
    if( yPos < init_yPos )
      init_yPos = yPos;
  //--


  int size = mnode->subnodes.size();
  for( int i =0; i < size; i++){
    vector3 localAttPos, laxis1, laxis2;
    obj_c = create( mnode->subnodes[i], l_pos, mnode->size, l_rot, C, &localAttPos, &laxis1, &laxis2 );
    obj_c->attach( obj_p, localAttPos, laxis1, laxis2 );
    //obj_c->attach( obj_p );
  }
  
  return obj_p;
}


body::body( dWorldID world, dSpaceID space, morf_node* root ){
  vector3 tmp;
  dMatrix3 I;
  this->World = world;
  this->Space = space;

  dRSetIdentity( I );
  init_yPos = 0;
  create( root, vector3(0,0,0), vector3(0,0,0), vector3(0,0,0), I, &tmp, &tmp, &tmp );

  
  int size = bodyParts.size();

  for( int i = 0; i < size; i++ ){
    bodyParts[i]->setPos( bodyParts[i]->getPos() + vector3(0,-init_yPos*1.4142,0) );
  }
}


body::~body(){

  int size = bodyParts.size();

  for( int i = 0; i < size; i++ ){
    delete bodyParts[i];
  }

  bodyParts.clear();
}







vector3 body::getPos(){
  vector3 pos;
  pos.assign(0,0,0);

  int size = bodyParts.size();

  for( int i = 0; i < size; i++ ){
    pos = pos + bodyParts[i]->getPos();
  }

  pos = pos / size;

  return pos;
}







float body::run( int time ){
  int size = bodyParts.size();
  double input[INPUT_LAYER];
  double output[OUTPUT_LAYER];

  float f1, f2;
  float penalty;

  for( int i = 0; i < size; i++ ){
    if( bodyParts[i]->hasJoint ){
      //dVector3 fu, fu2;
      //dJointGetUniversalAxis1 ( bodyParts[i]->jid, fu);
      //dJointGetUniversalAxis1 ( bodyParts[i]->jid, fu2);


      input[0] = dJointGetUniversalAngle1(bodyParts[i]->jid);
      input[1] = dJointGetUniversalAngle2(bodyParts[i]->jid);
      input[2] = bodyParts[i]->collide_floor;
      input[3] = bodyParts[i]->collide_body;

      bodyParts[i]->collide_floor = false;
      bodyParts[i]->collide_body = false;

      input[4] = sin(time*PI/(double)20);
      
      const dReal *R = dGeomGetRotation( bodyParts[i]->geom);

      input[5] = atan2( R[6], R[5] );
      input[6] = atan2( R[8], R[0] );
      input[7] = atan2( R[1], R[0] );


      bodyParts[i]->nn->Run( input, output );

      f1 = output[0];
      f2 = output[1];

      float vol1 = bodyParts[i]->volume;
      float vol2 = vol1;
      if( bodyParts[i]->dad )
	vol2 = bodyParts[i]->dad->volume;

      float max_rate;
      float max_force;

      if( vol1 > vol2 ){
	max_force = MAX_FORCE(vol2);
	max_rate = MAX_RATE(vol2);
      }else{
	max_force = MAX_FORCE(vol1);
	max_rate = MAX_RATE(vol1);
      }

      /* RATE CONTROL
      if( f1 > 0 ){
	if( f1 > max_rate )
	  f1 = max_rate;
      }else{
	if( f1 < -max_rate )
	  f1 = -max_rate;
      }

      if( f2 > 0 ){
	if( f2 > max_rate )
	  f2 = max_rate;
      }else{
	if( f2 < -max_rate )
	  f2 = -max_rate;
      }
      */

      bodyParts[i]->force1 += f1;
      bodyParts[i]->force2 += f2;


      /* FORCE CONTROL
      if( bodyParts[i]->force1 > 0 ){
	if( bodyParts[i]->force1 > max_force )
	  bodyParts[i]->force1 = max_force;
      }else{
	if( bodyParts[i]->force1 < -max_force )
	  bodyParts[i]->force1 = -max_force;
      }

      if( bodyParts[i]->force2 > 0 ){
	if( bodyParts[i]->force2 > max_force )
	  bodyParts[i]->force2 = max_force;
      }else{
	if( bodyParts[i]->force2 < -max_force )
	  bodyParts[i]->force2 = -max_force;
      }
      */



      //printf( "%f %f \n", bodyParts[i]->force1, bodyParts[i]->force2 );

      /*
      float rateFactor[2];      
      bodyParts[i]->getRateFactor( rateFactor );
      bodyParts[i]->force1 *= rateFactor[0];
      bodyParts[i]->force2 *= rateFactor[1];
#define JOINT_RESTORE 100
      if( input[0] < -(PI/2) ) bodyParts[i]->force1 = JOINT_RESTORE;
      if( input[0] > (PI/2) ) bodyParts[i]->force1 = -JOINT_RESTORE;

      if( input[1] < -(PI/2) ) bodyParts[i]->force2 = JOINT_RESTORE;
      if( input[1] > (PI/2) ) bodyParts[i]->force2 = -JOINT_RESTORE;
      static float fu = 0;
      fu += 0.01f;
      f1 = 100*cos(fu);
      f2 = 500*sin(fu);
      */

#ifdef DEBUG
      static double fu = 0;
      fu += 0.05;
      dJointAddUniversalTorques( bodyParts[i]->jid, 300*sin(fu), 300*cos(fu));
#else
      dJointAddUniversalTorques( bodyParts[i]->jid, bodyParts[i]->force1, bodyParts[i]->force2);
#endif

    }
  }

  return penalty;
}





