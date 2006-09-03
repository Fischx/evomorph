
#include "vector3.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vector3::vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

vector3::vector3(float X , float Y, float Z){
	x = X;
	y = Y;
	z = Z;
}

vector3::~vector3()
{

}

//-----------------------------------------------------------------------------
// Desc: Muda os valores das vars XYZ
//-----------------------------------------------------------------------------
void vector3::assign(float X , float Y, float Z){
	x = X;
	y = Y;
	z = Z;
}



//-----------------------------------------------------------------------------
// Desc: verfica se o vetor e um vetor nulo
//-----------------------------------------------------------------------------
bool vector3::isNull()
{
	if( x == 0 && y == 0 && z== 0 )
		return true;
	return false;
}


//-----------------------------------------------------------------------------
// Desc: Retorna a distancia entre o vetor e o ponto 0,0,0
//-----------------------------------------------------------------------------
float vector3::mod(){
	return (float)sqrt( x*x + y*y + z*z );
}


//-----------------------------------------------------------------------------
// Desc: Retorna a distancia (ao quadrado) entre o vetor e o ponto 0,0,0
//-----------------------------------------------------------------------------
float vector3::mod2(){
	return ( x*x + y*y + z*z );
}


//-----------------------------------------------------------------------------
// Desc: Faz com q os valores XYZ do vetor fiquem entre 0 e 1 (apontando na mesma direcao)
//-----------------------------------------------------------------------------
vector3 vector3::norm(){
	register float foo = 1/((float)sqrt( x*x + y*y + z*z ));
	x*=foo;
	y*=foo;
	z*=foo;
	return *this;
}


//-----------------------------------------------------------------------------
// Desc: Calcula a distancia entre o vetor fornecido e o vetor local
//-----------------------------------------------------------------------------
float vector3::dist( vector3* vec ){
	register vector3 distVec = vector3( vec->x - this->x, vec->y - this->y, vec->z - this->z );
	return (float)sqrt( distVec.x*distVec.x + distVec.y*distVec.y + distVec.z*distVec.z );
}





///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

//-----------------------------------------------------------------------------
// Desc: Soma de vetores
//-----------------------------------------------------------------------------
vector3 vector3::operator+(const vector3 &a){
	return ( vector3(this->x + a.x, this->y + a.y , this->z + a.z) );
}

//-----------------------------------------------------------------------------
// Desc: Subtracao de vetores
//-----------------------------------------------------------------------------
vector3 vector3::operator-(const vector3 &a){
	return ( vector3(this->x - a.x, this->y - a.y , this->z - a.z) );
}

//-----------------------------------------------------------------------------
// Desc: Multiplicacao de um vetor por um numero escalar
//-----------------------------------------------------------------------------
vector3 vector3::operator*(const float &a){
	return ( vector3(this->x * a , this->y * a , this->z * a) );
}

//-----------------------------------------------------------------------------
// Desc: Divisao de um vetor por um numero escalar
//-----------------------------------------------------------------------------
vector3 vector3::operator/(const float &a){
	register float inv = 1 / a;
	return ( vector3(this->x * inv , this->y * inv , this->z * inv) );
}

//-----------------------------------------------------------------------------
// Desc: Multiplicacao escalar de vetores
//-----------------------------------------------------------------------------
float vector3::operator*(const vector3 &a){
	return ( this->x*a.x + this->y*a.y + this->z*a.z );
}

//-----------------------------------------------------------------------------
// Desc: Multiplicacao Vetorial (Cross Product)
//-----------------------------------------------------------------------------
vector3 vector3::operator^(const vector3 &a){
	return vector3( (this->y * a.z - this->z * a.y) , (this->z * a.x - this->x * a.z) , (this->x * a.y - this->y * a.x) );
}

//-----------------------------------------------------------------------------
// Desc: Igualdade entre vetores
//-----------------------------------------------------------------------------
bool vector3::operator==(const vector3 &a){
	if( (fabs(this->x-a.x)<EPSILON) && (fabs(this->y-a.y)<EPSILON) && (fabs(this->z-a.z)<EPSILON) )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Desc: Desigualdade entre vetores
//-----------------------------------------------------------------------------
bool vector3::operator!=(const vector3 &a){
	if( (fabs(this->x-a.x)>EPSILON) || (fabs(this->y-a.y)>EPSILON) || (fabs(this->z-a.z)>EPSILON) )
	{
		return true;
	}
	return false;
}

vector3 vector3::rotate(const vector3 &v, double ang){
  // First we calculate [w,x,y,z], the rotation quaternion
  double w,x,y,z;
  vector3 V=v;
  V.norm();
  w=cos(-ang/2);  // The formula rotates counterclockwise, and I
                  // prefer clockwise, so I change 'ang' sign

  double s=sin(-ang/2);
  x=V.x*s;
  y=V.y*s;
  z=V.z*s;

  // now we calculate [w^2, x^2, y^2, z^2]; we need it
  double w2=w*w;
  double x2=x*x;
  double y2=y*y;
  double z2=z*z;
   
  // And apply the formula
  return vector3((*this).x*(w2+x2-y2-z2) + (*this).y*2*(x*y+w*z)   + (*this).z*2*(x*z-w*y),
		 (*this).x*2*(x*y-w*z)   + (*this).y*(w2-x2+y2-z2) + (*this).z*2*(y*z+w*x),
		 (*this).x*2*(x*z+w*y)   + (*this).y*2*(y*z-w*x)   + (*this).z*(w2-x2-y2+z2));
}
