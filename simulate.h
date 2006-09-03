#ifndef _SIMULATE_H_
#define _SIMULATE_H_



#include <ode/ode.h>


#include "graphics.h"
#include "events.h"

#include "vector3.h"
#include "substratum.h"
//#include "graphics.h"

#include "body.h"


#define MAX_CONTACTS 16



// Below is some defines about getting fitness

//#define WAIT_STEPS 1000
//#define FITNESS_STEPS 100
//#define TOTAL_STEPS 50000
#define TOTAL_STEPS 10000
#define FITNESS_STEPS (TOTAL_STEPS/2)
#define WAIT_STEPS (TOTAL_STEPS/5)

class simulate
{

 public:
  float getFitness( body* creature );
  void show( body* creature );
  void step();

  simulate();
  ~simulate();



  static dWorldID World;
  static dSpaceID Space;
  static dJointGroupID contactgroup;
  events * evt;
  graphics * gr;

 private:



  static void collideCallback (void *data, dGeomID o1, dGeomID o2);


};

#endif
