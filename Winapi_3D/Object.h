#pragma once
class Object
{
private:
	bool selected = false;
	int shape;
	cVector3 position;
	cVector3 rotatoin;
	cVector3 scale;
	
	std::vector<cVector3> localVec3;
	std::vector<cVector3> RenderPos;
	
	bool SetCube();
	bool SetTetrahedron();
	bool SetGrid();
	
public:
	Object(float x, float y, float z);
	Object(int shape);
	Object(float x, float y, float z, int shape);
	~Object();
	bool Render(HDC hdc, cVector3& vEye, cVector3& vLookat);
	bool Pipeline(cVector3& vEye, cVector3& vLookat);
	bool Select(bool Click);

	bool Move(float amount, int enumXYZ);
	bool Rotate(float amount, int enumXYZ);
	bool Reform(float amount, int enumXYZ);

	cVector3& GetPosition();
};




