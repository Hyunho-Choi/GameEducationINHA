#include "stdafx.h"
#include "cVector3.h"


cVector3::cVector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

cVector3::cVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


cVector3::~cVector3()
{
}

bool cVector3::operator==(cVector3 & vec)
{
	if (((this->x + E >= vec.x) && (this->x - E <= vec.x)) &&
		((this->x + E >= vec.y) && (this->x - E <= vec.y)) &&
		((this->x + E >= vec.z) && (this->x - E <= vec.z)))
		return true;
	return false;
}

bool cVector3::operator!=(cVector3 & vec)
{
	if (((this->x + E >= vec.x) && (this->x - E <= vec.x)) &&
		((this->x + E >= vec.y) && (this->x - E <= vec.y)) &&
		((this->x + E >= vec.z) && (this->x - E <= vec.z)))
		return false;
	return true;
}

cVector3 cVector3::operator+(cVector3 & vec)
{
	cVector3 cal = vec;
	cal.x += this->x;
	cal.y += this->y;
	cal.z += this->z;

	return cal;
}

cVector3 cVector3::operator-(cVector3 & vec)
{
	cVector3 cal;
	cal.x = this->x - vec.x;
	cal.y = this->y - vec.y;
	cal.z = this->z - vec.z;
	

	return cal;
}

bool cVector3::operator < (float num)
{
	if ((pow(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2), 0.5)) < num)
		return true;
	else
		return false;
}

cVector3 cVector3::operator*(float multi)
{
	cVector3 cal = *this;
	cal.x *= multi;
	cal.y *= multi;
	cal.z *= multi;

	return cal;
}


cVector3 cVector3::operator/(float multi)
{
	cVector3 cal = *this;
	cal.x /= multi;
	cal.y /= multi;
	cal.z /= multi;

	return cal;
}


float cVector3::Dot(cVector3 & v1, cVector3 & v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);

}

cVector3 cVector3::Cross(cVector3 & v1, cVector3 & v2)
{
	cVector3 cal;
	cal.x = v1.y * v2.z - v1.z * v2.y;
	cal.y = v1.z * v2.x - v1.x * v2.z;
	cal.z = v1.x * v2.y - v1.y * v2.x;

	return cal;
}

float cVector3::Angle(cVector3 & v1, cVector3 & v2)
{
	return acosf(cVector3::Dot(v1, v2) / (v1.Length() * v2.Length())) * (180 / PI);
}

cVector3 cVector3::TriangleVec(cVector3 & v1, cVector3 & v2, cVector3 & v3) 
{
	return cVector3::Cross(v1 - v2, v1 - v3).Normalize();
}

cVector3 cVector3::TransformCoord(cVector3& v, cMatrix& mat)  // 점이동
{
	float w = 1.f;
	float mX, mY, mZ;

	mX = v.GetX() * mat[0][0] + v.GetY() * mat[1][0] + v.GetZ() * mat[2][0] + v.w * mat[3][0];
	mY = v.GetX() * mat[0][1] + v.GetY() * mat[1][1] + v.GetZ() * mat[2][1] + v.w * mat[3][1];
	mZ = v.GetX() * mat[0][2] + v.GetY() * mat[1][2] + v.GetZ() * mat[2][2] + v.w * mat[3][2];
	w = v.GetX() * mat[0][3] + v.GetY() * mat[1][3] + v.GetZ() * mat[2][3] + v.w * mat[3][3];
	
	if (w != 0)
	{
		cVector3 result(mX / w, mY / w, mZ / w);
		return result;
	}
	else
	{
		cVector3 result(mX, mY, mZ);
		return result;
	}
	//v.w = 1.f;

	// 선생님 코드
	/*
	cVector3 vRet;
	vRet.x = v.x * mat[0][0] + v.y*mat[1][0] +
		v.z * mat[2][0] + 1.0f * mat[3][0];
	vRet.y = v.x * mat[0][1] + v.y*mat[1][1] +
		v.z * mat[2][1] + 1.0f * mat[3][1];
	vRet.y = v.x * mat[0][2] + v.y*mat[1][2] +
		v.z * mat[2][2] + 1.0f * mat[3][2];
	float w = v.x * mat[0][3] + v.y*mat[1][3] +
		v.z * mat[2][3] + 1.0f * mat[3][3];

	if (-E > w || w > E)
	{
		vRet.x /= w;
		vRet.y /= w;
		vRet.z /= w;
	}
	
	return vRet;
	*/
}

cVector3 cVector3::TransformNormal(cVector3& v, cMatrix& mat)  // 벡터이동
{
	float w = 0.f;

	float mX, mY, mZ;

	mX = v.GetX() * mat[0][0] + v.GetY() * mat[1][0] + v.GetZ() * mat[2][0];
	mY = v.GetX() * mat[0][1] + v.GetY() * mat[1][1] + v.GetZ() * mat[2][1];
	mZ = v.GetX() * mat[0][2] + v.GetY() * mat[1][2] + v.GetZ() * mat[2][2];

	cVector3 result(mX, mY, mZ);
	return result;

	// 선생님 코드
	/*
	cVector3 vRet;
	vRet.x = v.x * mat[0][0] + v.y*mat[1][0] +
	v.z * mat[2][0] + 1.0f * mat[3][0];
	vRet.y = v.x * mat[0][1] + v.y*mat[1][1] +
	v.z * mat[2][1] + 1.0f * mat[3][1];
	vRet.y = v.x * mat[0][2] + v.y*mat[1][2] +
	v.z * mat[2][2] + 1.0f * mat[3][2];
	float w = v.x * mat[0][3] + v.y*mat[1][3] +
	v.z * mat[2][3] + 1.0f * mat[3][3];
	
	return vRet;
	*/
}

float cVector3::Length() const
{
	return pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.5);
}

cVector3 cVector3::Normalize(float size) const
{
	cVector3 cal;

	const float len = pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.5) / size;
	cal.x = this->x / len;
	cal.y = this->y / len;
	cal.z = this->z / len;

	return cal;
}

void cVector3::Print() const
{
	std::cout << "{ " << x << " , " << y << " , " << z << " }\n";
}

float cVector3::GetX() const
{
	return this->x;
}

float cVector3::GetY() const
{
	return this->y;
}

float cVector3::GetZ() const
{
	return this->z;
}

bool cVector3::SetX(float X)
{
	this->x = X;
	return true;
}

bool cVector3::SetY(float Y)
{
	this->y = Y;
	return true;
}

bool cVector3::SetZ(float Z)
{
	this->z = Z;
	return true;
}
