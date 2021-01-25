#pragma once
class Enemy
{
private:
	POINT mPos = { 0,0 };
	int mAvatar;
	float mDirection;
	int mLife;
	int mIntDir;
	int mState = IDLE;
	Bullet mBullet[BULLET_CNT];

	int bx, by;
	HBITMAP HBit, HOldBit;
	BITMAP Bit;
	int MoveBx, MoveBy;
	int aniMoveX, aniMoveY;
	HBITMAP HIdleBit, HOldIdleBit;
	BITMAP IdleBit;
	int IdleBx, IdleBy;
	int aniIdleX, aniIdleY;

	int frameCnt = 0;
	

public:
	void SetIntDir(int dir);
	POINT GetPos();
	void SetParameter(PacketServerToClient newPara);
	void Draw(HDC hdc);
	void DrawGame(HDC hdc);
	Enemy();
	virtual ~Enemy();
};

