#include "stdafx.h"
#include "cMatrix.h"


cMatrix::cMatrix()
{
}


cMatrix::~cMatrix()
{

}

cMatrix::cRow::cRow()
{
}

cMatrix::cRow::cRow(int nDimention)
{
	for (int i = 0; i < nDimention; ++i)
		this->m_vecData.push_back(0);
}

cMatrix::cRow::~cRow()
{
	
}

void cMatrix::cRow::Resize(int nDimention)
{
	this->m_vecData.clear();
	for (int i = 0; i < nDimention; ++i)
		this->m_vecData.push_back(0);
}

float & cMatrix::cRow::operator[](int nIndex)
{
	return this->m_vecData[nIndex];
	// TODO: insert return statement here
}

cMatrix::cMatrix(int nDimension)
{
	for (int i = 0; i < nDimension; ++i)
	{
		cRow temp(nDimension);
		this->cCol.push_back(temp);
		cCol[i][i] = 1;
	}
}

void cMatrix::Set(float a[])
{
	int index = 0;
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			this->cCol[i][j] = a[index++];
		}
	}
}

void cMatrix::Print()
{
	if (cCol.size() == 0)
	{
		std::cout << "크기가 0입니다." << std::endl;
		return;
	}
	const int lastIndex = cCol.size() -1;

	for (int i = 0; i < cCol.size(); i++)
	{
		if(i == 0)
			std::cout << "┌\t";
		else if (i == cCol.size()-1)
			std::cout << "└\t";
		else
			std::cout << "│\t";

		for (int j = 0; j < cCol.size(); j++)
		{
			//std::cout << this->cCol[i][j] << "\t";
			printf("% .2f\t", this->cCol[i][j]);
		}

		if (i == 0)
			std::cout << "┐\n";
		else if (i == cCol.size() - 1)
			std::cout << "┘\n";
		else
			std::cout << "│\n";
	}



}

void cMatrix::Resize(int nDimension)
{
	cCol.clear();

	for (int i = 0; i < nDimension; ++i)
	{
		cRow temp(nDimension);
		this->cCol.push_back(temp);
		cCol[i][i] = 1;
	}
}

int cMatrix::Dimension()
{
	return this->cCol.size();
}

cMatrix cMatrix::Identity(int nDimension)
{
	cMatrix result;
	for (int i = 0; i < nDimension; ++i)
	{
		cRow temp(nDimension);
		result.cCol.push_back(temp);
		result.cCol[i][i] = 1;
	}

	return result;
}

cMatrix::cRow & cMatrix::operator[](int nIndex)
{
	return this->cCol[nIndex];
	// TODO: insert return statement here
	
}


bool cMatrix::operator==(cMatrix & mat)
{
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			if (this->cCol[i][j] != mat.cCol[i][j])
				return false;
		}
	}
	return true;
}

bool cMatrix::operator!=(cMatrix & mat)
{
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			if (this->cCol[i][j] != mat.cCol[i][j])
				return true;
		}
	}
	return false;
}

cMatrix cMatrix::operator+(cMatrix & mat)
{
	cMatrix result(this->cCol.size());

	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			result.cCol[i][j] = this->cCol[i][j] + mat.cCol[i][j];
		}
	}
	return result;
}

cMatrix cMatrix::operator-(cMatrix & mat)
{
	cMatrix result(this->cCol.size());
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			result.cCol[i][j] = this->cCol[i][j] - mat.cCol[i][j];
		}
	}
	return result;
}

cMatrix cMatrix::operator*(cMatrix & mat)
{
	cMatrix result(this->cCol.size());
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			float input = 0;
			for (int k = 0; k < cCol.size(); k++)
			{
				input += this->cCol[i][k] * mat.cCol[k][j];
			}
			if(abs(input) > E)
				result.cCol[i][j] = input;
			else
				result.cCol[i][j] = 0;
		}
	}
	return result;
}

cMatrix cMatrix::operator*(float f)
{
	cMatrix result(this->cCol.size());
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			result.cCol[i][j] = this->cCol[i][j] * f;
		}
	}
	return result;
}

cMatrix cMatrix::Transpose() //전치행렬
{
	cMatrix result(this->cCol.size());
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			result.cCol[i][j] = this->cCol[j][i];
		}
	}
	return result;
}

cMatrix cMatrix::Invers() //역행렬
{
	int strat = GetTickCount();
	cMatrix result(this->cCol.size());
	result = (this->Adjoint() * (1.0f / this->Determinant(this->cCol.size())));
	int end = GetTickCount();
	std::cout << "경과시간 : "<<end - strat << std::endl;
	return result;
}

float cMatrix::Determinant(int num)
{
	float result = 0;
	for (int i = 0; i < num; i++)
	{
		if (num == 1)
		{
			return this->cCol[0][0];
		}

		if (num == 2)
		{
			return this->Minor(0, i)[0][0];
		}
		else
		{
			if(i % 2)
				result -= (this->cCol[0][i] * this->Minor(0, i).Determinant(num - 1));
			else
				result += (this->cCol[0][i] * this->Minor(0, i).Determinant(num - 1));
		}
	}
	if (result == 0)
		std::cout << "역행렬이 존재하지 않습니다\n";
	return result;
}

cMatrix cMatrix::Adjoint()
{
	

	cMatrix result(this->cCol.size());
	for (int i = 0; i < cCol.size(); i++)
	{
		for (int j = 0; j < cCol.size(); j++)
		{
			if((i + j) % 2)
				result.cCol[i][j] = -(this->Minor(i, j).MinorCal(this->cCol.size()-1));
			else
				result.cCol[i][j] = this->Minor(i, j).MinorCal(this->cCol.size()-1);
		}
	}

	return result.Transpose();
}

float cMatrix::Cofactor(int nCol, int nRow)
{
	return 0.0f;
}

cMatrix cMatrix::Minor(int nCol, int nRow)
{
	cMatrix result(this->cCol.size()-1);

	if (result.cCol.size() < 1)
	{
		std::cout << "Minor Error\n";
		return this->cCol[0][0];
		//return reslt;
	}
		

	if (result.cCol.size() == 1)
	{
		result.cCol[0][0] = this->cCol[0][0] * this->cCol[1][1] - this->cCol[1][0] * this->cCol[0][1];
		return result;
	}

	int minorI = 0;
	int minorJ = 0;
	for (int i = 0; i < cCol.size(); i++)
	{
		if (i == nCol)
			continue;
		for (int j = 0; j < cCol.size(); j++)
		{
			if (j == nRow)
				continue;
			result.cCol[minorI][minorJ++] = this->cCol[i][j];
		}
		minorI++;
		minorJ = 0;
	}
	return result;
}

float cMatrix::MinorCal(int num)
{
	float result = 0;
	for (int i = 0; i < num; i++)
	{
		if (num == 1)
		{
			return this->cCol[0][0];
		}
		if (num == 2)
		{
			return this->Minor(0, i)[0][0];
		}
		else
		{
			if (i % 2)
				result -= (this->cCol[0][i] * this->Minor(0, i).MinorCal(num - 1));
			else
				result += (this->cCol[0][i] * this->Minor(0, i).MinorCal(num - 1));
		}
	}
	return result;
}

cMatrix cMatrix::Translation(float x, float y, float z)
{
	cMatrix tran(4);
	float arr[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1 };
	
	tran.Set(arr);
	return tran;
}

cMatrix cMatrix::Translation(cVector3& v)
{
	cMatrix tran(4);
	float arr[16] = {
		1,		0,			0,			0,
		0,		1,			0,			0,
		0,		0,			1,			0,
		v.GetX(), v.GetY(), v.GetZ(),	1 };

	tran.Set(arr);
	return tran;

	//선생님 코드
	/*
	cMatrix tran(4);

	tran[3][0] = v.GetX();
	tran[3][1] = v.GetY();
	tran[3][2] = v.GetZ();

	tran.Set(arr);
	return tran;
	*/
}

cMatrix cMatrix::RotationX(float Angle)
{
	cMatrix tran(4);
	float arr[16] = {
		1, 0,					0,					0,
		0, cosf(RAD(Angle)),	sinf(RAD(Angle)),	0,
		0, -sinf(RAD(Angle)), cosf(RAD(Angle)),	0,
		0, 0,					0,					1 };
	
	tran.Set(arr);
	return tran;
}

cMatrix cMatrix::RotationY(float Angle)
{
	cMatrix tran(4);
	float arr[16] = {
		cosf(RAD(Angle)),	0,	-sinf(RAD(Angle)),	0,
		0,					1,	0,					0,
		sinf(RAD(Angle)),	0,	cosf(RAD(Angle)),	0,
		0,					0,	0,					1 };

	tran.Set(arr);
	return tran;
}

cMatrix cMatrix::RotationZ(float Angle)
{
	cMatrix tran(4);
	float arr[16] = {
		cosf(RAD(Angle)),	sinf(RAD(Angle)),	0,	0,
		-sinf(RAD(Angle)),cosf(RAD(Angle)),0,	0,
		0,					0,					1,	0,
		0,					0,					0,	1 };

	tran.Set(arr);
	return tran;
}

cMatrix cMatrix::RotationRight(float Angle, cVector3 right)
{
	return RotationX(Angle * right.GetX()) * RotationZ(Angle * right.GetZ());
}

cMatrix cMatrix::Rotation(cVector3& Ro)
{
	return cMatrix::RotationX(Ro.GetX()) *
		cMatrix::RotationY(Ro.GetY()) *
		cMatrix::RotationY(Ro.GetZ());
}

cMatrix cMatrix::View(cVector3& vEye, cVector3& vLookAt, cVector3& vUp)
{
	/*
	ViewMatrix
		*	eye, lookat, up
		*	l = look vector
		*	r = right vector
		*	u = up vector
		*		r.x			u.x			l.x			0
		*		r.y			u.y			l.y			0
		*		r.z			u.z			l.z			0
		*		-r dot eye -u dot eye -l dot eye	1
	*/

	cVector3 l(vLookAt.GetX() - vEye.GetX(), vLookAt.GetY() - vEye.GetY(), vLookAt.GetZ() - vEye.GetZ());
	l = l.Normalize();
	
	cVector3 r = cVector3::Cross(vUp,l).Normalize();
	cVector3 u = cVector3::Cross(l,r).Normalize();

	cMatrix result(4);
	float arr[16] = {
		r.GetX(), u.GetX(), l.GetX(),	0,
		r.GetY(), u.GetY(), l.GetY(),	0,
		r.GetZ(), u.GetZ(), l.GetZ(),	0,
		-cVector3::Dot(r, vEye) , -cVector3::Dot(u, vEye) , -cVector3::Dot(l, vEye) , 1
	};

	result.Set(arr);
	return result;
}

cMatrix cMatrix::Projection(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	/*
	*Projection
		*	sy = cot(fovY / 2) = >	1.0f / tanf(fFovY / 2.0f)
		*	sx = sy / aspect
		*			sx		0		0					0
		*			0		sy		0					0
		*			0		0		fz / (fz - nz)		1
		*			0		0 - fz * nz / (fz - nz)		0
	*/

	float sy = 1.0f / tanf(fFovY / 2.0f);
	float sx = sy / fAspect;
	
	cMatrix result(4);
	float arr[16] = {
		sx,		0,		0,									0,
		0,		sy,		0,									0,
		0,		0,		fFarZ / (fFarZ - fNearZ),			1,
		0,		0,		(-fFarZ) * fNearZ / (fFarZ - fNearZ),	0,
	};

	result.Set(arr);
	return result;
}

cMatrix cMatrix::Viewport(float x, float y, float w, float h, float minz, float maxz)
{
	/*
	*Viewport
		*	w / 2.0f		0				0			0
		* 0					- h / 2.0f		0			0
		* 0					0				maxz - minz	0
		* x + (w / 2.0f)	y + (h / 2.0f)	minz		1
		*
		*	default->maxz = 1, minz = 0
		*
	*/
	cMatrix result(4);
	float arr[16] = {
		w / 2.0f,		0,				0,			0,
		0,				-h / 2.0f,		0,			0,
		0,				0,				maxz - minz,0,
		x + (w / 2.0f),	y + (h / 2.0f),	minz,		1,
	};

	result.Set(arr);
	return result;
	
}

cVector3 cMatrix::PipeLine(cVector3& in, cVector3& vEye, cVector3& vLookAt)
{
	static cVector3 Up(0.f, 1.f, 0.f);
	static cMatrix Default = cMatrix::Projection() * cMatrix::Viewport(0, 0, (float)WIN_WIDE, (float)WIN_HIGHT);
	
	cMatrix pipe = (cMatrix::View(vEye, vLookAt, Up) * Default);
	return cVector3::TransformCoord(in, pipe);

	/*
	result = cVector3::TransformCoord(in, cMatrix::View(vEye, vLookAt, Up));
	result = cVector3::TransformCoord(result, cMatrix::Projection());
	result = cVector3::TransformCoord(result, cMatrix::Viewport(0, 0, (float)WIN_WIDE, (float)WIN_HIGHT));
	*/
}


