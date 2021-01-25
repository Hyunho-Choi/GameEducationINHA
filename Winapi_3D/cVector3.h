#pragma once
//#include "cMatrix.h"

class cMatrix;

class cVector3
{

public:
	cVector3();
	cVector3(float x, float y, float z );
	~cVector3();


private:
	float x, y, z;


public:
	float w = 1.f;
	bool operator==(cVector3& vec);
	bool operator!=(cVector3& vec);
	cVector3 operator+(cVector3& vec);
	cVector3 operator-(cVector3& vec);
	bool operator<(float num);
	cVector3 operator*(float multi);
	cVector3 operator/(float multi);

	
	static float Dot(cVector3& v1, cVector3& v2);
	static cVector3 Cross(cVector3& v1, cVector3& v2);
	static float Angle(cVector3& v1, cVector3& v2);
	static cVector3 TriangleVec(cVector3& v1, cVector3& v2, cVector3& v3) ;

	static cVector3 TransformCoord(cVector3& v, cMatrix& mat); // 점이동
	static cVector3 TransformNormal(cVector3& v, cMatrix& mat); // vector이동

	
	float Length() const;
	cVector3 Normalize(float size = 1.f) const;

	void Print() const;

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	bool SetX(float X) ;
	bool SetY(float Y) ;
	bool SetZ(float Z) ;
};

