#include "stdafx.h"
#include "Astar_Rect.h"

using namespace std;

int Astar_Rect::GetH(POINT pos)
{
	if (mAstarMap[pos.y][pos.x].H != -1)
		return mAstarMap[pos.y][pos.x].H;

	int result = 0;
	POINT Cal = {mEndPos.x - pos.x, mEndPos.y - pos.y};
	while ((Cal.x != 0) && (Cal.y != 0))
	{
		Cal.x -= (Cal.x / abs(Cal.x));
		Cal.y -= (Cal.y / abs(Cal.y));
		result += DEFALT_DIST_CROSS;
	}
	while (!((Cal.x == 0) && (Cal.y == 0)))
	{
		if(Cal.y == 0)
			Cal.x -= (Cal.x / abs(Cal.x));
		else
			Cal.y -= (Cal.y / abs(Cal.y));
		result += DEFALT_DIST;
	}

	mAstarMap[pos.y][pos.x].H = result;
	return result;
}

int Astar_Rect::GetG(POINT pos)
{
	if (mAstarMap[pos.y][pos.x].G == -1)
		mAstarMap[pos.y][pos.x].G++;

	if ((nowPos.x != pos.x) && (nowPos.y != pos.y))
	{
		mAstarMap[pos.y][pos.x].G += (DEFALT_DIST * mAstarMap[pos.y][pos.x].wight);
	}
	else
	{
		mAstarMap[pos.y][pos.x].G += (DEFALT_DIST_CROSS * mAstarMap[pos.y][pos.x].wight);
	}
	return mAstarMap[pos.y][pos.x].G;
}

int Astar_Rect::GetG2(POINT pos)
{
	if (mAstarMap[pos.y][pos.x].G == -1)
	{
		mAstarMap[pos.y][pos.x].G++;
		mAstarMap[pos.y][pos.x].G = mAstarMap[nowPos.y][nowPos.x].G + (DEFALT_DIST * mAstarMap[pos.y][pos.x].wight);
	}
	else if ((nowPos.x != pos.x) && (nowPos.y != pos.y))
	{
		int temp = mAstarMap[nowPos.y][nowPos.x].G + (DEFALT_DIST * mAstarMap[pos.y][pos.x].wight);
		if(temp < mAstarMap[pos.y][pos.x].G)
			mAstarMap[pos.y][pos.x].G = temp;
	}
	else
	{
		int temp = mAstarMap[nowPos.y][nowPos.x].G + (DEFALT_DIST_CROSS * mAstarMap[pos.y][pos.x].wight);
		if (temp < mAstarMap[pos.y][pos.x].G)
			mAstarMap[pos.y][pos.x].G = temp;
	}

	return mAstarMap[pos.y][pos.x].G;
}

void Astar_Rect::SetF(POINT pos)
{
	if ((pos.x >= MAP_SLICE) || (pos.y >= MAP_SLICE))
		return;

	if (mAstarMap[pos.y][pos.x].closed)
	{
		return;
	}
	mAstarMap[pos.y][pos.x].F = GetG2(pos) + GetH(pos);

	if (!mAstarMap[pos.y][pos.x].opened)
	{
		if ((pos.x >= 0) && (pos.y >= 0))
		{
			mOpenPos.push_back(pos);
			mAstarMap[pos.y][pos.x].opened = true;
		}
	}
}

void Astar_Rect::SetG(POINT center)
{
	int cent = mAstarMap[center.y][center.x].G;
	if(!mAstarMap[center.y - 1][center.x].closed)
		mAstarMap[center.y - 1][center.x].G = cent + DEFALT_DIST;

	if (!mAstarMap[center.y + 1][center.x].closed)
		mAstarMap[center.y + 1][center.x].G = cent + DEFALT_DIST;

	if (!mAstarMap[center.y][center.x - 1].closed)
		mAstarMap[center.y ][center.x - 1].G = cent + DEFALT_DIST;

	if (!mAstarMap[center.y][center.x + 1].closed)
		mAstarMap[center.y ][center.x + 1].G = cent + DEFALT_DIST;

	if (!mAstarMap[center.y - 1][center.x - 1].closed)
		mAstarMap[center.y - 1][center.x - 1].G = cent + DEFALT_DIST_CROSS;

	if (!mAstarMap[center.y - 1][center.x + 1].closed)
		mAstarMap[center.y - 1][center.x + 1].G = cent + DEFALT_DIST_CROSS;

	if (!mAstarMap[center.y + 1][center.x - 1].closed)
		mAstarMap[center.y + 1][center.x - 1].G = cent + DEFALT_DIST_CROSS;

	if (!mAstarMap[center.y + 1][center.x + 1].closed)
		mAstarMap[center.y + 1][center.x + 1].G = cent + DEFALT_DIST_CROSS;
}

Astar_Rect::Astar_Rect()
{
	/*
	mAstarMap[4][4].wight = INF_WIGHT;
	mAstarMap[3][4].wight = INF_WIGHT;
	mAstarMap[5][4].wight = INF_WIGHT;
	mAstarMap[5][3].wight = INF_WIGHT;
	mAstarMap[5][2].wight = INF_WIGHT;
	mAstarMap[2][4].wight = INF_WIGHT;
	mAstarMap[2][3].wight = INF_WIGHT;
	mAstarMap[2][2].wight = INF_WIGHT;
	mAstarMap[1][4].wight = INF_WIGHT;
	mAstarMap[0][4].wight = INF_WIGHT;
	*/
	nowPos = mStartPos;

}


Astar_Rect::~Astar_Rect()
{
}

void Astar_Rect::SetRect(RECT& window_size)
{
	if (window_size.right > window_size.bottom)
		mWindowSize = { 0,0, window_size.bottom ,window_size.bottom };
	else
		mWindowSize = { 0,0, window_size.right ,window_size.right };

	mPixelSize = mWindowSize.right / MAP_SLICE;
}

bool Astar_Rect::SetStartPos(POINT setStart)
{
	if ((setStart.x < mWindowSize.right) && (setStart.y < mWindowSize.right))
	{
		mEndPos.x = (setStart.x / mPixelSize);
		mEndPos.y = (setStart.y / mPixelSize);
		return true;
	}
	else
		return false;
}

POINT Astar_Rect::GetStartPos()
{
	return mStartPos;
}

bool Astar_Rect::SetEndPos(POINT setEnd)
{
	if ((setEnd.x < mWindowSize.right) && (setEnd.y < mWindowSize.right))
	{
		mEndPos.x = (setEnd.x / mPixelSize);
		mEndPos.y = (setEnd.y / mPixelSize);
		return true;
	}
	else
		return false;
}

POINT Astar_Rect::GetEndPos()
{
	return mEndPos;
}

void Astar_Rect::SetAstar()
{
	
	while (1)
	{
		if ((nowPos.x == mEndPos.x) && (nowPos.y == mEndPos.y))
			break;

		if (mClosePos.size() == 0)
		{
			//시작점
			//SetG(mStartPos);
			SetF({ mStartPos.x + 1 , mStartPos.y });
			SetF({ mStartPos.x - 1 , mStartPos.y });
			SetF({ mStartPos.x , mStartPos.y + 1 });
			SetF({ mStartPos.x , mStartPos.y - 1 });
			SetF({ mStartPos.x + 1 , mStartPos.y + 1 });
			SetF({ mStartPos.x + 1 , mStartPos.y - 1 });
			SetF({ mStartPos.x - 1 , mStartPos.y + 1 });
			SetF({ mStartPos.x - 1 , mStartPos.y - 1 });

			mClosePos.push_back(mStartPos);
			mAstarMap[mStartPos.y][mStartPos.x].closed = true;
		}
		else
		{
			// while
			// open중에 제일 작은값으로 start
			POINT now;
			std::vector<POINT> Same;

			for (int i = 0; i < mOpenPos.size(); i++)
			{
				if (!mAstarMap[mOpenPos[i].y][mOpenPos[i].x].closed)
				{
					now = mOpenPos[i];
					break;
				}
			}

			for (int i = 0; i < mOpenPos.size(); i++)
			{
				if (!mAstarMap[mOpenPos[i].y][mOpenPos[i].x].closed)
				{
					if (mAstarMap[now.y][now.x].F > mAstarMap[mOpenPos[i].y][mOpenPos[i].x].F)
					{
						Same.clear();
						now = mOpenPos[i];
						Same.push_back(mOpenPos[i]);
					}
					else if (mAstarMap[now.y][now.x].F == mAstarMap[mOpenPos[i].y][mOpenPos[i].x].F)
					{
						Same.push_back(mOpenPos[i]);
					}
				}
			}
			nowPos = now;
			
			for (int i = 0; i < Same.size(); i++)
			{
				if (mAstarMap[Same[i].y][Same[i].x].H < mAstarMap[nowPos.y][nowPos.x].H)
					nowPos = Same[i];
			}
			
			//SetG(nowPos);

			SetF({ nowPos.x + 1 , nowPos.y });
			SetF({ nowPos.x , nowPos.y + 1 });
			SetF({ nowPos.x , nowPos.y - 1 });
			SetF({ nowPos.x - 1 , nowPos.y });
			SetF({ nowPos.x + 1 , nowPos.y + 1 });
			SetF({ nowPos.x + 1 , nowPos.y - 1 });
			SetF({ nowPos.x - 1 , nowPos.y + 1 });
			SetF({ nowPos.x - 1 , nowPos.y - 1 });

			mClosePos.push_back(nowPos);
			mAstarMap[nowPos.y][nowPos.x].closed = true;
		}
	}
}

void Astar_Rect::DrawAstarTile(HDC hdc)
{

	for (int i = 0; i < MAP_SLICE; i++)
	{
		for (int j = 0; j < MAP_SLICE; j++)
		{
			if (mAstarMap[i][j].opened)
			{
				HBRUSH StartBrush = CreateSolidBrush(RGB(0, 255, 0));
				HBRUSH OldBrush;
				OldBrush = (HBRUSH)SelectObject(hdc, StartBrush);

				Rectangle(hdc, j * mPixelSize, i * mPixelSize, j * mPixelSize + mPixelSize, i * mPixelSize + mPixelSize);

				SelectObject(hdc, OldBrush);

				DeleteObject(StartBrush);
				DeleteObject(OldBrush);
			}

			if (mAstarMap[i][j].closed)
			{
				HBRUSH StartBrush = CreateSolidBrush(RGB(255, 0, 0));
				HBRUSH OldBrush;
				OldBrush = (HBRUSH)SelectObject(hdc, StartBrush);

				Rectangle(hdc, j * mPixelSize, i * mPixelSize, j * mPixelSize + mPixelSize, i * mPixelSize + mPixelSize);

				SelectObject(hdc, OldBrush);

				DeleteObject(StartBrush);
				DeleteObject(OldBrush);
			}

			if ((mStartPos.y == i) && (mStartPos.x == j))
			{
				HBRUSH StartBrush = CreateSolidBrush(RGB(255, 255, 0));
				HBRUSH OldBrush;
				OldBrush = (HBRUSH)SelectObject(hdc, StartBrush);

				Rectangle(hdc, j * mPixelSize, i * mPixelSize, j * mPixelSize + mPixelSize, i * mPixelSize + mPixelSize);

				SelectObject(hdc, OldBrush);

				DeleteObject(StartBrush);
				DeleteObject(OldBrush);
			}

			if ((mEndPos.y == i) && (mEndPos.x == j))
			{
				HBRUSH StartBrush = CreateSolidBrush(RGB(255, 0, 255));
				HBRUSH OldBrush;
				OldBrush = (HBRUSH)SelectObject(hdc, StartBrush);

				Rectangle(hdc, j * mPixelSize, i * mPixelSize, j * mPixelSize + mPixelSize, i * mPixelSize + mPixelSize);

				SelectObject(hdc, OldBrush);

				DeleteObject(StartBrush);
				DeleteObject(OldBrush);
			}

			
			if (mAstarMap[i][j].wight == INF_WIGHT)
			{
				HBRUSH StartBrush = CreateSolidBrush(RGB(10, 10, 10));
				HBRUSH OldBrush;
				OldBrush = (HBRUSH)SelectObject(hdc, StartBrush);

				Rectangle(hdc, j * mPixelSize, i * mPixelSize, j * mPixelSize + mPixelSize, i * mPixelSize + mPixelSize);

				SelectObject(hdc, OldBrush);

				DeleteObject(StartBrush);
				DeleteObject(OldBrush);
			}
			
			TextOut(hdc, j * mPixelSize, i * mPixelSize, 
				to_wstring(mAstarMap[i][j].G).c_str(), _tcslen(to_wstring(mAstarMap[i][j].G).c_str()));
			TextOut(hdc, j * mPixelSize + 26, i * mPixelSize,
				to_wstring(mAstarMap[i][j].H).c_str(), _tcslen(to_wstring(mAstarMap[i][j].H).c_str()));
			TextOut(hdc, j * mPixelSize + 13, i * mPixelSize + 15,
				to_wstring(mAstarMap[i][j].F).c_str(), _tcslen(to_wstring(mAstarMap[i][j].F).c_str()));
		}
	}

	for (int i = 0; i < MAP_SLICE + 1; i++)
	{
		MoveToEx(hdc, i * mPixelSize, 0, NULL);
		LineTo(hdc, i * mPixelSize, mWindowSize.right);

		MoveToEx(hdc, 0, i * mPixelSize, NULL);
		LineTo(hdc, mWindowSize.right, i * mPixelSize);
	}

}

void Astar_Rect::CreateWall(POINT wall)
{
	if ((wall.x < mWindowSize.right) && (wall.y < mWindowSize.right))
	{
		mAstarMap[wall.y / mPixelSize][wall.x / mPixelSize].wight = INF_WIGHT;
		return;
	}
	else
		return;
}
