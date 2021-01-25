#include "stdafx.h"


void Enemy::SetIntDir(int dir)
{
	mIntDir = dir;
}

POINT Enemy::GetPos()
{
	return mPos;
}

void Enemy::SetParameter(PacketServerToClient newPara)
{
	mIntDir = newPara.IntDir;
	mDirection = newPara.direction;
	mPos = newPara.pos;
	mLife = newPara.life;
	mState = newPara.state;
	memcpy(mBullet, newPara.myBullet, sizeof(mBullet));
}

void Enemy::Draw(HDC hdc)
{
	
	Rectangle(hdc, mPos.x - (MAPTILE_SIZE * 0.5), mPos.y - (MAPTILE_SIZE * 0.5), 
		mPos.x + (MAPTILE_SIZE * 0.5), mPos.y + (MAPTILE_SIZE * 0.5));

	for (int i = 0; i < BULLET_CNT; i++)
	{
		mBullet[i].Draw(hdc);
	}
}

void Enemy::DrawGame(HDC hdc)
{
	Rectangle(hdc, XMARGIN + (mPos.x - (MAPTILE_SIZE * 0.3))* GAME_MULTIFY, WIN_SIZEY * 0.5 - ((MAPTILE_SIZE * 0.3)* GAME_MULTIFY),
		XMARGIN + (mPos.x + (MAPTILE_SIZE * 0.3))* GAME_MULTIFY, WIN_SIZEY * 0.5 + ((MAPTILE_SIZE * 0.3)* GAME_MULTIFY));
	
	HDC HMemDC = CreateCompatibleDC(hdc);
	//Rectangle(hdc, mRt.left, mRt.top, mRt.right, mRt.bottom);
	

	switch (mState)
	{
	case IDLE:
		switch (mIntDir)
		{
		case DOWN:
			aniIdleY = 0;
			break;
		case UP:
			aniIdleY = (IdleBy / 4) * 1;
			break;
		case LEFT:
			aniIdleY = (IdleBy / 4) * 2;
			break;
		case RIGHT:
			aniIdleY = (IdleBy / 4) * 3;
			break;
		}
		HOldIdleBit = (HBITMAP)SelectObject(HMemDC, HIdleBit);
		TransparentBlt(hdc,
			mPos.x * GAME_MULTIFY + XMARGIN - (IdleBx / 10), WIN_SIZEY * 0.5 - (IdleBy / 4) + 20,
			(int)(IdleBx / 5), IdleBy / 4,
			HMemDC, aniIdleX, aniIdleY, (int)(IdleBx / 5), IdleBy / 4, RGB(255, 0, 255));
		if (frameCnt++ > 0)
		{
			frameCnt = 0;
			aniIdleX += (int)(IdleBx / 5);
			if (aniIdleX >= IdleBx)
				aniIdleX = 0;
		}
		SelectObject(HMemDC, HOldIdleBit);

		break;
	case MOVE:
		switch (mIntDir)
		{
		case DOWN:
			aniMoveY = 0;
			break;
		case UP:
			aniMoveY = (MoveBy / 4) * 1;
			break;
		case LEFT:
			aniMoveY = (MoveBy / 4) * 2;
			break;
		case RIGHT:
			aniMoveY = (MoveBy / 4) * 3;
			break;
		}
		HOldBit = (HBITMAP)SelectObject(HMemDC, HBit);
		TransparentBlt(hdc,
			mPos.x * GAME_MULTIFY + XMARGIN - (MoveBx / 12), WIN_SIZEY * 0.5 - (MoveBy / 4) + 20,
			(int)(MoveBx / 6), MoveBy / 4,
			HMemDC, aniMoveX, aniMoveY, (int)(MoveBx / 6), MoveBy / 4, RGB(255, 0, 255));
		if (frameCnt++ > 0)
		{
			frameCnt = 0;
			aniMoveX += (int)(MoveBx / 6);
			if (aniMoveX >= MoveBx)
				aniMoveX = 0;
		}
		SelectObject(HMemDC, HOldBit);
		break;
	case FIRE:
		break;
	case DEMEGED:
		break;
	}
	
	
	DeleteDC(HMemDC);


	for (int i = 0; i < BULLET_CNT; i++)
	{
		mBullet[i].DrawGame(hdc);
	}
}

Enemy::Enemy()
{
	HBit = (HBITMAP)LoadImage(NULL, TEXT("image/Walk_Down.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(HBit, sizeof(BITMAP), &Bit);
	MoveBx = Bit.bmWidth;
	MoveBy = Bit.bmHeight;

	HIdleBit = (HBITMAP)LoadImage(NULL, TEXT("image/Idle_Colt.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(HIdleBit, sizeof(BITMAP), &IdleBit);
	IdleBx = IdleBit.bmWidth;
	IdleBy = IdleBit.bmHeight;



	aniMoveX = 0;
	aniMoveY = 0;
	aniIdleX = 0;
	aniIdleY = 0;
}


Enemy::~Enemy()
{

}
