
#include "vector2.h"

//----------------------------------------------------------------------------
// Desc: Construtor
//----------------------------------------------------------------------------
vector2::vector2()
{
	x = 0;
	y = 0;
	quantRotacionada=0;
}

//----------------------------------------------------------------------------
// Desc: Construtor atribuindo
//----------------------------------------------------------------------------
vector2::vector2(float X, float Y)
{
	x = X;
	y = Y;
	quantRotacionada=0;
}


//----------------------------------------------------------------------------
// Desc: Destruidor
//----------------------------------------------------------------------------
vector2::~vector2()
{

}

//----------------------------------------------------------------------------
// Desc: Atribuicao
//----------------------------------------------------------------------------
 void vector2::assign(float X, float Y) {
	x = X;
	y = Y;
}

//-----------------------------------------------------------------------------
// Desc: verfica se o vetor e um vetor nulo
//-----------------------------------------------------------------------------
 bool vector2::isNull()
{
	if( x==0 && y==0 )
		return true;
	return false;
}


//-----------------------------------------------------------------------------
// Desc: Calcula a distancia entre o vetor fornecido e o vetor local
//-----------------------------------------------------------------------------
 float vector2::dist( vector2* vec ){
	float X = (x - vec->x);
	float Y = (y - vec->y);
	return (float)sqrt( X * X + Y * Y );
}


//-----------------------------------------------------------------------------
// Desc: Retorna a distancia entre o vetor e o ponto 0,0
//-----------------------------------------------------------------------------
 float vector2::mod(){
	return (float)sqrt( x*x + y*y );
}

//-----------------------------------------------------------------------------
// Desc: Retorna a distancia (ao quadrado) entre o vetor e o ponto 0,0
//-----------------------------------------------------------------------------
 float vector2::mod2(){
	return ( x*x + y*y );
}

//-----------------------------------------------------------------------------
// Desc: Faz com q a Norma fique igual a 1
//-----------------------------------------------------------------------------
 void vector2::norm(){
	float foo = 1/((float)sqrt( x*x + y*y ));
	x*=foo;
	y*=foo;
}


//-----------------------------------------------------------------------------
// Desc: Girar o vetor no angulo fornecido em radianos
//-----------------------------------------------------------------------------
 void vector2::rot( float Ang )
{
	float senB = (float)sin( Ang );
	float cosB = sqrt(1-senB*senB);
	float Y = this->y;	
	this->y = (this->y * cosB) + (this->x * senB);
	this->x = (this->x * cosB) - ( Y * senB);
	quantRotacionada+=Ang;
}

void vector2::rotOrigem( float Ang ){
	rot(2*Pi-quantRotacionada);
	quantRotacionada=0;
	rot(Ang);
}


/*Informa o angulo entre dois vetores*/
float vector2::ang(vector2* vec){
	vector2 v(x-vec->x,y-vec->y);
	vector2 u(0,y-vec->y);
	
	if(y>vec->y)
		return acos(u*v/((float)sqrt( v.x*v.x + v.y*v.y )*(float)sqrt( u.x*u.x + u.y*u.y )));
	else
		return 2*Pi-acos(u*v/((float)sqrt( v.x*v.x + v.y*v.y )*(float)sqrt( u.x*u.x + u.y*u.y )));
}


//-----------------------------------------------------------------------------
/////////                 OPERADORES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Desc: Desigualdade entre vetores
//-----------------------------------------------------------------------------
 bool vector2::operator!=(const vector2 &a){
	if( (fabs(this->x-a.x)>EPSILON) || (fabs(this->y-a.y)>EPSILON) )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Desc: Igualdade entre vetores
//-----------------------------------------------------------------------------
 bool vector2::operator==(const vector2 &a){
	if( (fabs(this->x-a.x)<EPSILON) && (fabs(this->y-a.y)<EPSILON) )
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Desc: Soma de vetores
//-----------------------------------------------------------------------------
 vector2 vector2::operator+(const vector2 &a){
	return ( vector2(this->x + a.x, this->y + a.y ) );
}

//-----------------------------------------------------------------------------
// Desc: Subtracao de vetores
//-----------------------------------------------------------------------------
 vector2 vector2::operator-(const vector2 &a){
	return ( vector2(this->x - a.x, this->y - a.y ) );
}



//-----------------------------------------------------------------------------
// Desc: Multiplicacao de um vetor por escalar (float)
//-----------------------------------------------------------------------------
 vector2 vector2::operator*(const float &a){
	return ( vector2(this->x * a , this->y * a ) );
}

//-----------------------------------------------------------------------------
// Desc: Multiplicacao Escalacar
//-----------------------------------------------------------------------------
 float vector2::operator*(const vector2 &a){
	return ( this->x*a.x + this->y*a.y );
}

//-----------------------------------------------------------------------------
// Desc: Divisao de um vetor por um escalar (float)
//-----------------------------------------------------------------------------
 vector2 vector2::operator/(const float &a){
	register float inv = 1.f / a;
	return ( vector2(this->x * inv , this->y * inv ) );
}
