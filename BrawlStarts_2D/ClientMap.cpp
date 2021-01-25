#include "stdafx.h"
#include "ClientMap.h"


void ClientMap::draw(HDC hdc)
{
	for (int j = 0; j < MAPSIZEY; j++)
	{
		for (int i = 0; i < MAPSIZEX; i++)
		{
			Rectangle(hdc, Map[j][i].rt.left, Map[j][i].rt.top, Map[j][i].rt.right, Map[j][i].rt.bottom);
			if (Map[j][i].state)
				Rectangle(hdc, Map[j][i].rt.left+3, Map[j][i].rt.top+3, Map[j][i].rt.right-3, Map[j][i].rt.bottom-3);
		}
	}
}

void ClientMap::DrawGame(HDC hdc, int y)
{
	int UserArea = y / MAPTILE_SIZE;
	int UserPixel = y % MAPTILE_SIZE;

	
	for (int j = 0; j < MAPSIZEY; j++)
	{
		for (int i = 0; i < MAPSIZEX; i++)
		{
			if (((Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 > WIN_SIZEY) ||
				(((Map[j][i].rt.bottom - y)* GAME_MULTIFY + WIN_SIZEY * 0.5) < 0))
				break;

			int bx, by;
			HDC HMemDC;
			switch (Map[j][i].state)
			{
			case STUMP:
				HMemDC = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				TransparentBlt(hdc, XMARGIN + Map[j][i].rt.left * GAME_MULTIFY , (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - 22,
					(Map[j][i].rt.right - Map[j][i].rt.left)* GAME_MULTIFY, by-45,
					HMemDC, 200, 0, 50, 74-45, RGB(255, 0, 255));
				DeleteDC(HMemDC);
				break;

			case ITMEBOX:
				HMemDC = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc, XMARGIN + Map[j][i].rt.left * GAME_MULTIFY, (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - 24,
					(Map[j][i].rt.right - Map[j][i].rt.left)* GAME_MULTIFY, by - 45 ,
					HMemDC, 100, 0, SRCCOPY);
				DeleteDC(HMemDC);
				break;

			case CACTUS:
				HMemDC = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				TransparentBlt(hdc, XMARGIN + Map[j][i].rt.left * GAME_MULTIFY-1, (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - 22,
					(Map[j][i].rt.right - Map[j][i].rt.left)* GAME_MULTIFY, by - 45,
					HMemDC, 150, 0, 50, 74 - 45, RGB(255, 0, 255));
				DeleteDC(HMemDC);
				break;
			//case OUTOFGAME:
			case ROCK:
				HMemDC = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc, XMARGIN + Map[j][i].rt.left * GAME_MULTIFY, (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - 24,
					(Map[j][i].rt.right - Map[j][i].rt.left)* GAME_MULTIFY, by - 45,
					HMemDC, 0, 0, SRCCOPY);
				DeleteDC(HMemDC);
			
				break;

			case BUSH:
				HMemDC = CreateCompatibleDC(hdc);
				hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				TransparentBlt(hdc, XMARGIN + Map[j][i].rt.left * GAME_MULTIFY, (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - 24,
					(Map[j][i].rt.right - Map[j][i].rt.left)* GAME_MULTIFY, by,
					HMemDC, 50, 0, 50,74, RGB(255,0,255));
				DeleteDC(HMemDC);

				break;
			default:
				/*
				Rectangle(hdc, XMARGIN + Map[j][i].rt.left* GAME_MULTIFY + TILE_MARGIN, (Map[j][i].rt.top - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 + TILE_MARGIN,
					XMARGIN + Map[j][i].rt.right* GAME_MULTIFY - TILE_MARGIN, (Map[j][i].rt.bottom - y)* GAME_MULTIFY + WIN_SIZEY * 0.5 - TILE_MARGIN);
				*/
				break;
			}
		}
	}



	/*
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (UserArea - i < 0)
				break;
			Rectangle(hdc, Map[UserArea-i][j].rt.left * multi, (Map[UserArea - i][j].rt.top + UserPixel) * multi,
				Map[UserArea - i][j].rt.left * multi + MAPTILE_SIZE * multi, (Map[UserArea - i][j].rt.top + UserPixel) * multi + MAPTILE_SIZE * multi);
		}
	}
	*/
}

void ClientMap::ImportMap()
{
	std::string str;
	std::ifstream loadFile;
	loadFile.open("map_index.csv");

	if (loadFile.is_open())
	{
		while (!loadFile.eof())
		{
			std::string temp;
			std::getline(loadFile, temp);
			str += temp;
			str += "\n";
		}
	}

	loadFile.close();

	std::string::iterator it = str.begin();
	std::string toInt;
	for (int i = 2; i < 42;)
	{
		for (int j = 2; j < 22; )
		{
			if (++it == str.end())
				return;
			switch ((*it))
			{
			case '\n' :
				Map[i][j].state = std::stoi(toInt);
				toInt.clear();
				++j;
				++i;
				break;
			case ',' :
				Map[i][j].state = std::stoi(toInt);
				toInt.clear();
				++j;
				break;
			default:
				toInt.push_back(*it);
				break;
			}
		}
	}

}

ClientMap::ClientMap()
{
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/all_tile.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap, sizeof(BITMAP), &bit);

	for (int i = 0; i < MAPSIZEY; i++)
	{
		for (int j = 0; j < MAPSIZEX; j++)
		{
			Map[i][j].rt = { j*MAPTILE_SIZE, i*MAPTILE_SIZE , j*MAPTILE_SIZE + MAPTILE_SIZE, i*MAPTILE_SIZE + MAPTILE_SIZE };
		}
	}


	for (int j = 5; j < MAPSIZEX - 5; j++)
	{
		Map[MAPSIZEY / 2][j].state = ROCK;
	}




	for (int j = 0; j < MAPSIZEX; j++)
	{
		Map[0][j].state = OUTOFGAME;
		Map[1][j].state = OUTOFGAME;
		Map[MAPSIZEY-2][j].state = OUTOFGAME;
		Map[MAPSIZEY-1][j].state = OUTOFGAME;
	}

	for (int j = 0; j < MAPSIZEY; j++)
	{
		Map[j][0].state = OUTOFGAME;
		Map[j][1].state = OUTOFGAME;
		Map[j][MAPSIZEX-2].state = OUTOFGAME;
		Map[j][MAPSIZEX-1].state = OUTOFGAME;
	}

	ImportMap();
}

ClientMap::~ClientMap()
{
	DeleteObject(hBitmap);
}
