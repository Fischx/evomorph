#ifndef _BODY_H_
#define _BODY_H_


#include <vector>

#include <ode/ode.h>

#include "vector3.h"
#include "genotype.h"
#include "substratum.h"
#include  <math.h>


using namespace std;





//max force proportional to arm's volume
//#define MAX_FORCE(vol) (((vol)*(vol))*25 + (vol)*30 + 14)
//#define MAX_FORCE(vol) ((pow(2,vol)*6) - (vol*vol*9) + vol*90)
//#define MAX_FORCE(vol) ((pow(2,vol)*3) + (vol*vol*17) + vol*20 + 5)

//#define MAX_FORCE(vol) ((pow(2,vol)) + (vol*vol*10) + vol*10 + 10)

//#define MAX_FORCE(vol) ((vol)*(vol)*(vol) + 10)



//#define MAX_FORCE(vol) (8*((3*(vol*vol*vol)+ 5*40*(vol*vol))/2) + 50)
//#define MAX_FORCE(vol) ((vol*100)+100)
//#define MAX_RATE(vol) (MAX_FORCE(vol)/100)




//#define MAX_FORCE(vol) ( 2*( 4*(vol*vol*vol) + 180*(vol*vol) +80*vol) + 100 )
#define MAX_FORCE(vol) 20*(4*((3*(vol*vol*vol)+ 5*40*(vol*vol))/2) + 20)
#define MAX_RATE(vol) (MAX_FORCE(vol)/(50*10))



//(2.^x)*3 + (x.^2)*17 + x*20 +5 
//(2.^x)*6 + -(x.^2)*9 + x*90

//  10.002   11.000   48.443  522.000
//  10.016   11.000   21.391   74.000





class body{


 public:
  body( dWorldID world, dSpaceID space, morf_node* root );
  ~body();

  float run(int time);


  vector< substratum* > bodyParts;


  vector3 getPos();

 private:
  substratum* create( morf_node* mnode, vector3 p_pos, vector3 p_size, vector3 p_rot, dMatrix3 p_MatRot, vector3* attach_pos, vector3* axis1, vector3* axis2 );


  dWorldID World;
  dSpaceID Space;

  float init_yPos;
};



#endif
