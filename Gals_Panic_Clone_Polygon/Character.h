#pragma once


using namespace std;

class Character
{
private:
	POINT myPos;
	POINT mySpeed;
	POINT lastPos;
	CirList nowArea;
	CirList newMakeArea;
	POINT newStartPos;
	POINT newLastPos;
	bool onMyArea;
	double DirAngle;
	int howManyturnRight;

	POINT test;
	bool testLight;


public:
	Character();
	~Character();

	void Update();
	void Draw(HDC hdc);
	void Controllor();
	bool IsPointInPolygon(static POINT point, static CirList& polygon);
	//bool IsPointInPolygon2(static POINT p, static list<POINT>& polygon);
	//void MergeNewMake(static CirList& nowArea, static CirList& newMakeArea);
	//void MergeNewMake2(static CirList& nowArea, static CirList& newMakeArea);
	double GetDistance(POINT A, POINT B);
};

