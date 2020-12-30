#pragma once

// ������(2) & �������� �ƴ� ���� ��� �̵�����
// �Ѱ迵���� ������ ���� ��ȯ
// �������� ��� �浹�� ���ӿ���

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

