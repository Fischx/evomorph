

#include "simulate.h"


dWorldID simulate::World;
dSpaceID simulate::Space;
dJointGroupID simulate::contactgroup;


simulate::simulate()
{
  World = dWorldCreate();

  Space = dSimpleSpaceCreate(0);

  contactgroup = dJointGroupCreate(0);
  
  dCreatePlane(Space, 0, 1, 0, 0);
  
  dWorldSetGravity(World, 0, -9.0, 0);

  dWorldSetERP(World, 0.7);

  dWorldSetCFM(World, 0.001f);//1e-2);

  dWorldSetContactMaxCorrectingVel(World, 10);//10
  
  dWorldSetContactSurfaceLayer(World, 0.001);

  //dWorldSetAutoDisableFlag(World, 1);

}


simulate::~simulate()
{
  dJointGroupDestroy(contactgroup);
  
  dSpaceDestroy(Space);
  
  dWorldDestroy(World);

}





static float collision_depth = 0;


float simulate::getFitness( body* creature ){
  int totalSteps = 0;
  int fitSteps = 0;
  float fitness = 0;

  vector3 oldpos, newpos;
  vector3 eoldpos, enewpos, creaturePos;


  while( 1 ){
    if( totalSteps > TOTAL_STEPS )
      break;

    creaturePos = creature->getPos();

    if( totalSteps > WAIT_STEPS ){
      if( fitSteps > FITNESS_STEPS ){
	newpos = creaturePos;
	fitness = fitness + newpos.dist( &oldpos );
	oldpos = newpos;
      }
      fitSteps++;
    }else{
      oldpos = creaturePos;
    }

    creature->run( totalSteps );
    collision_depth = 0;
    this->step();
    if( collision_depth > 1 )
      fitness -= collision_depth;

    totalSteps++;
  }

  return fitness;
}




void simulate::step()
{
  dSpaceCollide(Space, 0, &collideCallback);
  //dWorldQuickStep(World, 0.05f );

  dWorldStep(World, 0.05f );

  dJointGroupEmpty(contactgroup);
}

void simulate::collideCallback (void *data, dGeomID o1, dGeomID o2)
{
  int numc;
  dBodyID b1;
  dBodyID b2;
  substratum* subs1;
  int floor;

  /* Create an array of dContact objects to hold the contact joints */
  dContact contact[MAX_CONTACTS];

  /* Temporary index for each contact */
  int i;

  /* Get the dynamics body for each geom */
  b1 = dGeomGetBody(o1);
  b2 = dGeomGetBody(o2);



  floor = 0;
  if( dGeomGetClass(o1) == dPlaneClass || dGeomGetClass(o2) == dPlaneClass )
    floor = 1;

  /* Now we set the joint properties of each contact. Going into the
     full details here would require a tutorial of its own. I'll just
     say that the members of the dContact structure control the joint
     behaviour, such as friction, velocity and bounciness. See section
     7.3.7 of the ODE manual and have fun experimenting to learn
     more. */
  for (i = 0; i < MAX_CONTACTS; i++)
    {
      contact[i].surface.mode = dContactBounce | dContactSoftCFM;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.mu2 = 0;
      contact[i].surface.bounce = 0.0001f;
      contact[i].surface.bounce_vel = 0.001f;
      contact[i].surface.soft_cfm = 0.001f;

      //contact[i].surface.bounce = 0.01;
      //contact[i].surface.bounce_vel = 0.1;
      //contact[i].surface.soft_cfm = 0.001;

    }

  /* Here we do the actual collision test by calling dCollide. It
     returns the number of actual contact points or zero if there were
     none. As well as the geom IDs, max number of contacts we also
     pass the address of a dContactGeom as the fourth
     parameter. dContactGeom is a substructure of a dContact object so
     we simply pass the address of the first dContactGeom from our
     array of dContact objects and then pass the offset to the next
     dContactGeom as the fifth paramater, which is the size of a
     dContact structure. That made sense didn't it? */
  if ( (numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) )
    {
      /* To add each contact point found to our joint group we call
	 dJointCreateContact which is just one of the many different
	 joint types available. */
      for (i = 0; i < numc; i++)
        {
	  collision_depth += contact[i].geom.depth;

	  /* dJointCreateContact needs to know which world and joint
	     group to work with as well as the dContact object
	     itself. It returns a new dJointID which we then use with
	     dJointAttach to finally create the temporary contact
	     joint between the two geom bodies. */

	  if( dGeomGetClass(o1) == dBoxClass ){ // It is a substratum
	    subs1 = (substratum*) dBodyGetData(b1);
	    if( dGeomGetClass(o2) == dPlaneClass )
	      subs1->collide_floor = true;
	    else
	      subs1->collide_body = true;
	  }

	  if( dGeomGetClass(o2) == dBoxClass ){ // It is a substratum
	    subs1 = (substratum*) dBodyGetData(b2);
	    if( dGeomGetClass(o1) == dPlaneClass )
	      subs1->collide_floor = true;
	    else
	      subs1->collide_body = true;
	  }
	  
	  //if( !(dGeomGetClass(o1) == dBoxClass && dGeomGetClass(o2) == dBoxClass ))
	  {
	    dJointID c;
	    c = dJointCreateContact(World, contactgroup, contact + i);
	    dJointAttach(c, b1, b2);
	  }

        }
    }
}




