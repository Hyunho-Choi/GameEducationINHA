#pragma once


class Astar_Rect
{
private:
	RECT mWindowSize;
	TILE mAstarMap[MAP_SLICE][MAP_SLICE];
	int mPixelSize;
	POINT mStartPos = {3,3};
	POINT mEndPos;
	POINT nowPos = mStartPos;
	std::vector<POINT> mClosePos;
	std::vector<POINT> mOpenPos;
	int GetH(POINT pos);
	int GetG(POINT pos);
	int GetG2(POINT pos);
	void SetF(POINT pos);
	void SetG(POINT center);

public:
	Astar_Rect();
	~Astar_Rect();
	void SetRect(RECT& window_size);

	bool SetStartPos(POINT setStart);
	POINT GetStartPos();
	bool SetEndPos(POINT setEnd);
	POINT GetEndPos();

	void SetAstar();
	
	void DrawAstarTile(HDC hdc);
	void CreateWall(POINT wall);


};

