
#if !defined(_VECTOR3_H_)
#define _VECTOR3_H_

# include <math.h>

#define EPSILON 0.0001f


class vector3
{
public:
	vector3();
	virtual ~vector3();
	vector3(float X , float Y, float Z);

	float x, y, z;	// Componentes do vetor (Posicao)


	// FUNCOES MEMBRO

	void assign(float X , float Y, float Z);
	bool isNull();
	float mod();
	float mod2();
	vector3 norm();
	float dist( vector3* vec );
	vector3 rotate(const vector3 &v, double ang);


	// OPERATORS

	vector3 operator+(const vector3 &a);
	vector3 operator-(const vector3 &a);
	vector3 operator*(const float &a);
	float operator*(const vector3 &a);
	vector3 operator/(const float &a);
	vector3 operator^(const vector3 &a);
	bool operator==(const vector3 &a);
	bool operator!=(const vector3 &a);
	

};

#endif
