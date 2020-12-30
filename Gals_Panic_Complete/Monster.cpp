#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
	mHeadPos = { (rand()%75 + 2) , (rand() % 75 + 2) };
	while(monMap.GetMap({ mHeadPos.x , mHeadPos.y }) == 2)
		mHeadPos = { (rand() % 75 + 2) , (rand() % 75 + 2) };

	mDrawPos.push_front({ mHeadPos.y , mHeadPos.x });
	mDirection = 3;
	mSpeed = 1;
	mDirChangeCnt = 0;
	delTriger = false;
	srand((unsigned int)time(NULL));
}


Monster::~Monster()
{
	
}

bool Monster::Update()
{
	this->Move();
	return this->CheckCollision();

}

bool Monster::CheckCollision()
{
	if (monMap.GetMap({ mHeadPos.x ,mHeadPos.y }) == 3)
	{
		// 충돌시 코드
		return true;
	}
	
	switch (mDirection)
	{
	case 1:
		if (monMap.GetMap({ mHeadPos.x-1 ,mHeadPos.y }) == 2)
			ReverseDir();
		break;
	case 2:
		if (monMap.GetMap({ mHeadPos.x  ,mHeadPos.y+1 }) == 2)
			ReverseDir();
		break;
	case 3:
		if (monMap.GetMap({ mHeadPos.x + 1 ,mHeadPos.y }) == 2)
			ReverseDir();
		break;
	case 4:
		if (monMap.GetMap({ mHeadPos.x  ,mHeadPos.y-1 }) == 2)
			ReverseDir();
		break;
	}

	if (monMap.GetMap({ mHeadPos.x ,mHeadPos.y }) == 2)
	{
		delTriger = true;
	}

	return false;
}

void Monster::Move()
{
	
	mDirChangeCnt++;

	mDrawPos.push_front({ mHeadPos.y ,mHeadPos.x });
	if (mDrawPos.size() > 10)
	{
		mDrawPos.pop_back();
	}
		

	switch (mDirection)
	{
	case 1:
		mHeadPos.x -= mSpeed;
		break;
	case 2:
		mHeadPos.y += mSpeed;
		break;
	case 3:
		mHeadPos.x += mSpeed;
		break;
	case 4:
		mHeadPos.y -= mSpeed;
		break;
	}


	if (mHeadPos.x < 2 || mHeadPos.x > MAPSIZE - 3)
	{
		ReverseDir();
	}
	if (mHeadPos.y < 2 || mHeadPos.y > MAPSIZE - 3)
	{
		ReverseDir();
	}

	if ((mHeadPos.y > 3 && mHeadPos.y < MAPSIZE - 4) && (mHeadPos.x > 3 && mHeadPos.x < MAPSIZE - 4))
	{
		if (mDirChangeCnt >= ((rand() % 5) + 20))
		{
			mDirection = (rand() % 4) + 1;
			mDirChangeCnt = 0;
		}
	}
}

void Monster::Draw(HDC hdc)
{

	MonPenHead = CreatePen(PS_INSIDEFRAME | PS_NULL, 0,RGB(49, 18, 115));
	MonPen2 = CreatePen(PS_INSIDEFRAME | PS_NULL, 0, RGB(5, 84, 242));

	MonBrushHead = CreateSolidBrush(RGB(49, 18, 115));
	MonBrush2 = CreateSolidBrush(RGB(5, 84, 242));

	moldPen = (HPEN)SelectObject(hdc, MonPen2);
	moldBrush = (HBRUSH)SelectObject(hdc, MonBrush2);

	int tempX = (PIXELSIZE*mHeadPos.y) + (PIXELSIZE / 2) - (CHARSIZE / 2);
	int tempY = (PIXELSIZE*mHeadPos.x) + (PIXELSIZE / 2) - (CHARSIZE / 2);


	for (int i = 0; i < mDrawPos.size(); i++)
	{
		Rectangle(hdc, mDrawPos[i].x * PIXELSIZE, mDrawPos[i].y * PIXELSIZE, 
			mDrawPos[i].x * PIXELSIZE + PIXELSIZE, mDrawPos[i].y * PIXELSIZE + PIXELSIZE);
	}

	SelectObject(hdc, MonPenHead);
	SelectObject(hdc, MonBrushHead);

	Ellipse(hdc, tempX, tempY, tempX + CHARSIZE, tempY + CHARSIZE);

	SelectObject(hdc, moldPen);
	SelectObject(hdc, moldBrush);

	DeleteObject(MonPenHead);
	DeleteObject(MonPen2);
	DeleteObject(moldPen);
	DeleteObject(MonBrushHead);
	DeleteObject(MonBrush2);
	DeleteObject(moldBrush);
}

void Monster::ReverseDir()
{
		if (mDirection == 1)
			mDirection = 3;
		else if (mDirection == 3)
			mDirection = 1;
	
		if (mDirection == 2)
			mDirection = 4;
		else if (mDirection == 4)
			mDirection = 2;
}
