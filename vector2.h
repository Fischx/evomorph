
#if !defined(_VECTOR2_H_)
#define _VECTOR2_H_


# include <math.h>



#define EPSILON 0.0001f
#define Pi 3.14159f

//-----------------------------------------------------------------------------
// CLASSE vector2 : Classe de Vetor em duas dimensoes
// Usada no plano 2D da movimentacao dos personagens pelo mapa
//-----------------------------------------------------------------------------
class vector2{	
	
	float quantRotacionada;
	
public:
	vector2();
	vector2(float X, float Y);	
	virtual ~vector2();


	// FUNCOES MEMBRO

	 void assign(float X, float Y);
	 bool isNull();
	 float dist( vector2* vec );
	 float mod();
	 float mod2();
	 void norm();
	 void rot( float Ang );
	 void rotOrigem( float Ang ); //Gira a partir da origem
	 float ang(vector2* vec);

	// OPERATORS

	 bool operator!=(const vector2 &a);
	 bool operator==(const vector2 &a);
	 vector2 operator+(const vector2 &a);
	 vector2 operator-(const vector2 &a);
	 vector2 operator*(const float &a);
	 float operator*(const vector2 &a);	
	 vector2 operator/(const float &a);
		
	float x; // Componente x
	float y; // Componente y

};

#endif 
