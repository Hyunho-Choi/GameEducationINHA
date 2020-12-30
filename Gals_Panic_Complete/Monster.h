#pragma once

// 내영역(2) & 범위밖이 아닌 곳은 모두 이동가능
// 한계영역에 도착시 방향 전환
// 새영역과 헤드 충돌시 게임오버

class Monster
{
private:
	deque<POINT> mDrawPos;
	POINT mHeadPos;
	int mDirection;
	int mSpeed;
	int mDirChangeCnt;
	Map monMap;
	
	HPEN moldPen;
	HPEN MonPenHead;
	HPEN MonPen2;
	HBRUSH moldBrush;
	HBRUSH MonBrushHead;
	HBRUSH MonBrush2;

public:
	bool delTriger;
	Monster();
	~Monster();
	bool Update();
	bool CheckCollision();
	void Move();
	void Draw(HDC hdc);
	void ReverseDir();
};

