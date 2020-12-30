#include "stdafx.h"
#include "Map.h"

short Map::map[MAPSIZE][MAPSIZE] = { 0 };

Map::Map()
{
	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			map[MAPSIZE / 2 - 3 + j][MAPSIZE / 2 - 3 + i] = 2;
		}
	}

	for (int i = 0; i < MAPSIZE; i++)
	{
		map[0][i] = 1;
		map[MAPSIZE-1][i] = 1;
		map[i][0] = 1;
		map[i][MAPSIZE - 1] = 1;
	}
}


Map::~Map()
{
}

bool Map::SetMap(ArrPoint positon, short val)
{
	map[positon.ArrX][positon.ArrY] = val;
	return false;
}

short Map::GetMap(ArrPoint positon)
{
	return map[positon.ArrX][positon.ArrY];
}

void Map::Update()
{
	
}

void Map::Draw(HDC hdc)
{

	blankBrush = CreateSolidBrush(RGB(107, 152, 242));
	blankPen = CreatePen(PS_INSIDEFRAME | PS_NULL, 0, RGB(107, 152, 242));

	myAreaBrush = CreateSolidBrush(RGB(255,0,255));
	myAreaPen = CreatePen(PS_INSIDEFRAME | PS_NULL, 0, RGB(255, 0, 255));

	newLineBrush = CreateSolidBrush(RGB(191, 109, 101));
	newLinePen = CreatePen(PS_INSIDEFRAME | PS_NULL, 0, RGB(191, 109, 101));

	int i, j;
	for (i = 0; i < MAPSIZE; i++)
	{
		for (j = 0; j < MAPSIZE; j++)
		{
			switch (map[j][i])
			{

			case 0:
				oldBrush = (HBRUSH)SelectObject(hdc, blankBrush);
				oldPen = (HPEN)SelectObject(hdc, blankPen);
				Rectangle(hdc, i * PIXELSIZE, j * PIXELSIZE, i * PIXELSIZE + PIXELSIZE, j * PIXELSIZE + PIXELSIZE);
				
				break;
			case 2:
				
				oldBrush = (HBRUSH)SelectObject(hdc, myAreaBrush);
				oldPen = (HPEN)SelectObject(hdc, myAreaPen);
				Rectangle(hdc, i * PIXELSIZE, j * PIXELSIZE, i * PIXELSIZE + PIXELSIZE, j * PIXELSIZE + PIXELSIZE);
		

				break;
			case 3:
				oldBrush = (HBRUSH)SelectObject(hdc, newLineBrush);
				oldPen = (HPEN)SelectObject(hdc, newLinePen);
				int tempX = (PIXELSIZE*i) + (PIXELSIZE / 2) - (CHARSIZE / 2);
				int tempY = (PIXELSIZE*j) + (PIXELSIZE / 2) - (CHARSIZE / 2);
				Rectangle(hdc, tempX, tempY, tempX + CHARSIZE, tempY + CHARSIZE);
				
				break;
			}
			
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
		}
	}

	DeleteObject(blankBrush);
	DeleteObject(blankPen);
	DeleteObject(myAreaBrush);
	DeleteObject(myAreaPen);
	DeleteObject(newLineBrush);
	DeleteObject(newLinePen);


}

bool Map::FillOutSide()
{
	// 새 최대 영역 스캔

	//왼쪽
	for (int i = 1; i < MAPSIZE - 1; i++)
	{
		for (int j = 1; j < MAPSIZE - 1; j++)
		{
			if (map[j][i] != 0)
			{
				MaxRect.left = i - 2;
				i = MAPSIZE - 2;
				break;
			}
		}
	}
	//오른쪽
	for (int i = MAPSIZE - 2; i > 0; i--)
	{
		for (int j = 1; j < MAPSIZE - 1; j++)
		{
			if (map[j][i] != 0)
			{
				MaxRect.right = i+2;
				i = 0;
				break;
			}
		}
	}
	//위
	for (int i = 1; i < MAPSIZE - 1; i++)
	{
		for (int j = 1; j < MAPSIZE - 1; j++)
		{
			if (map[i][j] != 0)
			{
				MaxRect.top= i - 2;
				i = MAPSIZE - 1;
				break;
			}
		}
	}
	//아래
	for (int i = MAPSIZE - 2; i > 0; i--)
	{
		for (int j = 1; j < MAPSIZE - 1; j++)
		{
			if (map[i][j] != 0)
			{
				MaxRect.bottom = i + 2;
				i = 0;
				break;
			}
		}
	}
	
	// 최대외부 영역 5로 초기화
	for (int i = MaxRect.left; i <= MaxRect.right; i++)
	{
		map[MaxRect.top][i] = 5;
		map[MaxRect.bottom][i] = 5;
	}
	for (int i = MaxRect.top; i <= MaxRect.bottom; i++)
	{
		map[i][MaxRect.left] = 5;
		map[i][MaxRect.right] = 5;
	}
	// 외부에서 플러드필 5
	
	FloodFillLast({ MaxRect.left + 1, MaxRect.top + 1 });

	// 최대영역안에서 6이아닌 모든 수 2로 초기화
	FillNotFive(MaxRect, 2 ,5);
	FillNotFive(MaxRect, 0, 2);

	for (int i = 0; i < MAPSIZE; i++)
	{
		map[0][i] = 1;
		map[MAPSIZE - 1][i] = 1;
		map[i][0] = 1;
		map[i][MAPSIZE - 1] = 1;
	}

	return false;
}

void Map::FloodFillLast(ArrPoint start)
{
	if ((map[start.ArrY][start.ArrX] != 0))
		return;

	map[start.ArrY][start.ArrX] = 5;
	FloodFillLast({ start.ArrX ,start.ArrY - 1 });
	FloodFillLast({ start.ArrX ,start.ArrY + 1 });
	FloodFillLast({ start.ArrX - 1 ,start.ArrY });
	FloodFillLast({ start.ArrX + 1 ,start.ArrY });
}

void Map::FillNotFive(RECT& Rt, int change, int not)
{
	for (int i = Rt.left; i <= Rt.right; i++)
	{
		for (int j = Rt.top; j <= Rt.bottom; j++)
		{
			if (map[j][i] != not)
			{
				map[j][i] = change;
			}
		}
	}
}

void Map::changeNewLine(ArrPoint start)
{

	if (map[start.ArrX][start.ArrY] == 3)
	{
		map[start.ArrX][start.ArrY] = 2;
		changeNewLine({ start.ArrX ,start.ArrY - 1 });
		changeNewLine({ start.ArrX ,start.ArrY + 1 });
		changeNewLine({ start.ArrX - 1 ,start.ArrY });
		changeNewLine({ start.ArrX + 1 ,start.ArrY });
	}
}

void Map::Reset4to2()
{
	for (int i = 1; i < MAPSIZE-1; i++)
	{
		for (int j = 1; j < MAPSIZE-1; j++)
		{
			if (map[i][j] == 4)
				map[i][j] = 2;
		}
	}
}

void Map::ResetAtoB(short A, short B)
{
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			if (map[i][j] == A)
				map[i][j] = B;
		}
	}
}


void Map::Restart()
{
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			map[i][j] = 0;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			map[MAPSIZE / 2 - 3 + j][MAPSIZE / 2 - 3 + i] = 2;
		}
	}

	for (int i = 0; i < MAPSIZE; i++)
	{
		map[0][i] = 1;
		map[MAPSIZE - 1][i] = 1;
		map[i][0] = 1;
		map[i][MAPSIZE - 1] = 1;
	}
}
