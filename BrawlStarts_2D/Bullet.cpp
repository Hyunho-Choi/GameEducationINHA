#include "stdafx.h"
//#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{

}

void Bullet::Draw(HDC hdc)
{
	// 총알 리소스마다 다른 그림 표시
	// 코드는 동일 = 같은 양의 그림파일 사용할 것

	if (isFire)
	{
		Rectangle(hdc, mPos.x - 5, mPos.y - 5, mPos.x + 5, mPos.y + 5);
	}
	
}

void Bullet::DrawGame(HDC hdc)
{
	if (isFire && (!mFireDelay))
	{
		Rectangle(hdc, XMARGIN + (mPos.x - (MAPTILE_SIZE * 0.2))* GAME_MULTIFY,
			WIN_SIZEY * 0.5 + ((GameY + MAPTILE_SIZE * 0.2)* GAME_MULTIFY),
			XMARGIN + (mPos.x + (MAPTILE_SIZE * 0.2))* GAME_MULTIFY,
			WIN_SIZEY * 0.5 + ((GameY - MAPTILE_SIZE * 0.2)* GAME_MULTIFY)
		);


	
	}

	
}

bool Bullet::isFired()
{
	return isFire;
}

POINT Bullet::GetPos()
{
	return mPos;
}

void Bullet::Reset()
{
	isFire = false;
}

void Bullet::Fire(POINT pos, int avatar, int bulletSpeed, float direction, int drawCnt, int delay)
{
	this->mPos = pos;
	this->mAvatar = avatar;
	this->mBulletSpeed = bulletSpeed;
	this->mDirection = direction;
	this->mDrawCnt = drawCnt;
	this->GameY = 0;
	this->mFireDelay = delay;

	isFire = true;
}

void Bullet::Update(POINT& user)
{
	if (isFire)
	{
		if (mFireDelay)
		{
			mFireDelay--;
			this->mPos = user;
			
			return;
		}

		mPos.x += mBulletSpeed*cos(mDirection);
		mPos.y += mBulletSpeed*sin(mDirection);
		GameY += mBulletSpeed*sin(mDirection);

		mDrawCnt--;
		if (mDrawCnt < 0)
			this->Reset();
	}
}
