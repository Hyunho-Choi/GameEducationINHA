#include "stdafx.h"
#include "Object.h"


bool Object::SetCube()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				localVec3.push_back(cVector3(pow(-1, i), pow(-1, j), pow(-1, k)));
				RenderPos.push_back(cVector3());
			}
		}
	}
	return true;
}

bool Object::SetTetrahedron()
{
	return true;
}

bool Object::SetGrid()
{
	for(int i = 0; i < 11; ++i )
	{
		localVec3.push_back(cVector3(i - 5, 0, 5));
		RenderPos.push_back(cVector3());
		localVec3.push_back(cVector3(i - 5, 0, -5));
		RenderPos.push_back(cVector3());
		localVec3.push_back(cVector3( 5, 0, i - 5 ));
		RenderPos.push_back(cVector3());
		localVec3.push_back(cVector3(-5, 0, i - 5));
		RenderPos.push_back(cVector3());
	}
	return true;
}

Object::Object(float x, float y, float z)
{
	this->position.SetX(x);
	this->position.SetY(y);
	this->position.SetZ(z);
}

Object::Object(int shape)
{
	this->scale.SetX(1);
	this->scale.SetY(1);
	this->scale.SetZ(1);
	
	this->shape = shape;
	
	switch (this->shape)
	{
	case CUBE:
		SetCube();
		break;
	case TETRAHEDRON:
		SetTetrahedron();
		break;
	case GRID:
		SetGrid();
		break;

	}
}

Object::Object(float x, float y, float z, int shape)
{
	this->position.SetX(x);
	this->position.SetY(y);
	this->position.SetZ(z);
	
	this->scale.SetX(1);
	this->scale.SetY(1);
	this->scale.SetZ(1);

	this->shape = shape;

	switch (this->shape)
	{
	case CUBE:
		SetCube();
		break;
	case TETRAHEDRON:
		SetTetrahedron();
		break;
	case GRID:
		SetGrid();
		break;
	}
}

bool Object::Render(HDC hdc, cVector3& vEye, cVector3& vLookat)
{
	Object::Pipeline(vEye, vLookat);

	switch (this->shape)
	{
	case CUBE:
		MoveToEx(hdc, RenderPos[0].GetX(), RenderPos[0].GetY(), NULL);
		LineTo(hdc, RenderPos[1].GetX(), RenderPos[1].GetY() );
		MoveToEx(hdc, RenderPos[0].GetX(), RenderPos[0].GetY(), NULL);
		LineTo(hdc, RenderPos[2].GetX(), RenderPos[2].GetY() );
		MoveToEx(hdc, RenderPos[0].GetX(), RenderPos[0].GetY(), NULL);
		LineTo(hdc, RenderPos[4].GetX(), RenderPos[4].GetY() );

		MoveToEx(hdc, RenderPos[5].GetX(), RenderPos[5].GetY(), NULL);
		LineTo(hdc, RenderPos[7].GetX(), RenderPos[7].GetY());
		MoveToEx(hdc, RenderPos[5].GetX(), RenderPos[5].GetY(), NULL);
		LineTo(hdc, RenderPos[4].GetX(), RenderPos[4].GetY());
		MoveToEx(hdc, RenderPos[5].GetX(), RenderPos[5].GetY(), NULL);
		LineTo(hdc, RenderPos[1].GetX(), RenderPos[1].GetY());

		MoveToEx(hdc, RenderPos[3].GetX(), RenderPos[3].GetY(), NULL);
		LineTo(hdc, RenderPos[2].GetX(), RenderPos[2].GetY());
		MoveToEx(hdc, RenderPos[3].GetX(), RenderPos[3].GetY(), NULL);
		LineTo(hdc, RenderPos[1].GetX(), RenderPos[1].GetY());
		MoveToEx(hdc, RenderPos[3].GetX(), RenderPos[3].GetY(), NULL);
		LineTo(hdc, RenderPos[7].GetX(), RenderPos[7].GetY());

		MoveToEx(hdc, RenderPos[6].GetX(), RenderPos[6].GetY(), NULL);
		LineTo(hdc, RenderPos[7].GetX(), RenderPos[7].GetY());
		MoveToEx(hdc, RenderPos[6].GetX(), RenderPos[6].GetY(), NULL);
		LineTo(hdc, RenderPos[4].GetX(), RenderPos[4].GetY());
		MoveToEx(hdc, RenderPos[6].GetX(), RenderPos[6].GetY(), NULL);
		LineTo(hdc, RenderPos[2].GetX(), RenderPos[2].GetY());

		break;

	case TETRAHEDRON:
		break;

	case GRID:
		int a = 0;
		int b = 2;
		for(int i= 0 ; i < 11; i++ )
		{
			MoveToEx(hdc, RenderPos[a].GetX(), RenderPos[a].GetY(), NULL);
			a++;
			LineTo(hdc, RenderPos[a].GetX(), RenderPos[a].GetY());
			a += 3;
			
			MoveToEx(hdc, RenderPos[b].GetX(), RenderPos[b].GetY(), NULL);
			b++;
			LineTo(hdc, RenderPos[b].GetX(), RenderPos[b].GetY());
			b += 3;
			
		}
		break;
		
	}


	return true;
}

bool Object::Pipeline(cVector3& vEye, cVector3& vLookat)
{
	// STR
	for(int i = 0; i < localVec3.size(); i++)
	{
		RenderPos[i] = cMatrix::PipeLine(
				cVector3::TransformCoord(
						this->localVec3[i],
					cMatrix::Rotation(this->rotatoin) * 
					cMatrix::Translation(this->position)),
				vEye, vLookat);
	}
	return true;
}

bool Object::Select(bool Click)
{
	this->selected = Click;
	return true;
}

Object::~Object()
{
}

bool Object::Move(float amount, int enumXYZ)
{
	if (this->shape == GRID)
		return false;
	switch (enumXYZ)
	{
	case enumX:
		position.SetX(position.GetX() + amount);
		break;

	case enumY:
		position.SetY(position.GetY() + amount);
		break;

	case enumZ:
		position.SetZ(position.GetZ() + amount);
		break;
	}
	return true;
}

bool Object::Rotate(float amount, int enumXYZ)
{
	if (this->shape == GRID)
		return false;
	switch (enumXYZ)
	{
	case enumX:
		rotatoin.SetX(rotatoin.GetX() + amount);
		break;

	case enumY:
		rotatoin.SetY(rotatoin.GetY() + amount);
		break;

	case enumZ:
		rotatoin.SetZ(rotatoin.GetZ() + amount);
		break;
	}
	return true;
}

bool Object::Reform(float amount, int enumXYZ)
{
	if (this->shape == GRID)
		return false;
	switch (enumXYZ)
	{
	case enumX:
		scale.SetX(scale.GetX() + amount);
		break;

	case enumY:
		scale.SetY(scale.GetY() + amount);
		break;

	case enumZ:
		scale.SetZ(scale.GetZ() + amount);
		break;
	}
	return true;

	
}

cVector3& Object::GetPosition()
{
	return this->position;
}
