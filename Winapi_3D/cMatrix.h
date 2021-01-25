#pragma once

class cMatrix
{
public:
	cMatrix();
	~cMatrix();

private:
	class cRow
	{
	private:
		std::vector<float> m_vecData;
	public:
		cRow();
		cRow(int nDimention);
		~cRow();
		void Resize(int nDimention);
		float& operator[](int nIndex);
	};
	std::vector<cRow> cCol;

public:
	
	cMatrix(int nDimension);
	void Set(float a[]);

	void Print();
	void Resize(int nDimension); //기본
	int Dimension();

	static cMatrix Identity(int nDimension); //표준

	cRow& operator[](int nIndex);
	
	bool operator==(cMatrix& mat);
	bool operator!=(cMatrix& mat);
	cMatrix operator+(cMatrix& mat);
	cMatrix operator-(cMatrix& mat);
	cMatrix operator*(cMatrix& mat);
	cMatrix operator*(float f);

	cMatrix Transpose();
	cMatrix Invers();
	float Determinant(int num);
	cMatrix Adjoint();
	float Cofactor(int nCol, int nRow);
	cMatrix Minor(int nCol, int nRow);

	float MinorCal(int num);

	// >> :
	static cMatrix Translation(float x, float y, float z);
	static cMatrix Translation(cVector3& v);
	static cMatrix RotationX(float Angle);
	static cMatrix RotationY(float Angle);
	static cMatrix RotationZ(float Angle);
	static cMatrix RotationRight(float Angle, cVector3 right);
	
	static cMatrix Rotation(cVector3& Ro);
	static cMatrix View(cVector3& vEye, cVector3& vLookAt, cVector3& vUp);
	static cMatrix Projection(float fFovY = PI / 4.0f, float fAspect = (float)WIN_WIDE / (float)WIN_HIGHT, float fNearZ = 1.0f, float fFarZ = 1000.0f);
	static cMatrix Viewport(float x, float y, float w, float h, float minz = 0.f, float maxz = 1.f);

	static cVector3 PipeLine(cVector3& in , cVector3& vEye, cVector3& vLookAt);
};
/*
 * ViewMatrix
 *	eye, lookat, up
 *	l = look vector
 *	r = right vector
 *	u = up vector
 *		r.x			u.x			l.x			0
 *		r.y			u.y			l.y			0
 *		r.z			u.z			l.z			0
 *		-r dot eye	-u dot eye	-l dot eye	0
 *
 *
 * Projection
 *	sy = cot(fovY / 2)		=>	1.0f / tanf(fFovY / 2.0f)
 *	sx = sy / aspect
 *			sx		0		0					0
 *			0		sy		0					0
 *			0		0		fz/(fz-nz)			1
 *			0		0		-fz * nz / (fz-nz)	0
 *
 *
 * Viewport
 *	w/2.0f		0			0			0
 *	0			-h / 2.0f	0			0
 *	0			0			maxz-minz	0
 *	x+(w/2.0f)	y+(h/2.0f)	minz		1
 *
 *	default -> maxz = 1, minz = 0
 * 
 */

