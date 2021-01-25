#include "stdafx.h"
//#include "LocalClient.h"


LocalClient::LocalClient()
{
	Enemy temp;
	Players.push_back(temp);
	Enemy temp2;
	Players.push_back(temp2);

	hBackBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/big_map2200.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	hBackBackBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/lobby_background.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	hIntroBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/Intro.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	GetObject(hBackBitmap, sizeof(BITMAP), &BackBit);
	GetObject(hBackBackBitmap, sizeof(BITMAP), &BackBackBit);
	GetObject(hIntroBitmap, sizeof(BITMAP), &IntroBit);
}


LocalClient::~LocalClient()
{
	DeleteObject(hBackBitmap);
	DeleteObject(hBackBackBitmap);
}

void LocalClient::Draw(HDC hdc)
{
	int start = GetTickCount();

	UIHdc = CreateCompatibleDC(hdc);
	
	bit = CreateCompatibleBitmap(hdc, WIN_SIZEX, WIN_SIZEY);
	oldbit = (HBITMAP)SelectObject(UIHdc, bit);

	//Rectangle(UIHdc, WIN_SIZEX - 100, -100, WIN_SIZEX + 100, WIN_SIZEY + 100); //대충배경

	if (IntroCnt > 0)
	{
		DrawIntro(UIHdc);	
		IntroCnt -= 2;
	}
	else
	{
		DrawBackground(UIHdc);
		//ClaMap.draw(UIHdc);
		
		for (std::vector<Enemy>::iterator it = Players.begin(); it != Players.end(); it++)
		{
			//(*it).Draw(UIHdc);
			(*it).DrawGame(UIHdc);
		}
		ClaMap.DrawGame(UIHdc, Players[0].GetPos().y);
	}

	BitBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, UIHdc, 0, 0, SRCCOPY);
	//TransparentBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, UIHdc, 0, 0, WIN_SIZEX, WIN_SIZEY, RGB(255, 0, 255));
	
	SelectObject(UIHdc, oldbit);
	DeleteObject(bit);
	DeleteDC(UIHdc);

	int end = GetTickCount();
	std::cout << end - start << std::endl;
}

void LocalClient::Update()
{
	Players[0].SetParameter(UserBuffer[0]);
	Players[1].SetParameter(UserBuffer[1]);
}

void LocalClient::DrawGame(HDC hdc)
{
	
}

void LocalClient::DrawBackground(HDC hdc)
{
	HDC HMemDC = CreateCompatibleDC(hdc);
	int bx, by;

	hOldBackBackBitmap = (HBITMAP)SelectObject(HMemDC, hBackBackBitmap);

	bx = BackBackBit.bmWidth;
	by = BackBackBit.bmHeight;

	BitBlt(hdc, 0,0, bx, by, HMemDC, 0, 0, SRCCOPY);

	hOldBackBitmap = (HBITMAP)SelectObject(HMemDC, hBackBitmap);
	
	bx = BackBit.bmWidth;
	by = BackBit.bmHeight;
	
	BitBlt(hdc, XMARGIN + 2 * MAPTILE_SIZE * GAME_MULTIFY - 100, WIN_SIZEY * 0.5 - Players[0].GetPos().y * GAME_MULTIFY -1, bx, by,
		HMemDC, 0, 0, SRCCOPY);
	/*
	BitBlt(hdc, 0, WIN_SIZEY * 0.5 - Players[0].GetPos().y * GAME_MULTIFY - 1, bx, by,
		HMemDC, 0, 0, SRCCOPY);
	*/
	//StretchBlt(hdc, 0, 0, bx, by, HMemDC, 120, Players[0].GetPos().y * GAME_MULTIFY * 0.61, 1100 , 1100, SRCCOPY);

	
	DeleteDC(HMemDC);

}

void LocalClient::DrawIntro(HDC hdc)
{
	HDC HMemDC = CreateCompatibleDC(hdc);
	int bx, by;

	hOldBackBackBitmap = (HBITMAP)SelectObject(HMemDC, hBackBackBitmap);

	bx = BackBackBit.bmWidth;
	by = BackBackBit.bmHeight;
	BitBlt(hdc, 0, 0, bx, by, HMemDC, 0, 0, SRCCOPY);

	hOldBackBitmap = (HBITMAP)SelectObject(HMemDC, hBackBitmap);

	bx = BackBit.bmWidth;
	by = BackBit.bmHeight;
	BitBlt(hdc, XMARGIN + 2 * MAPTILE_SIZE * GAME_MULTIFY - 100, WIN_SIZEY * 0.5 - (Players[0].GetPos().y - IntroCnt) * GAME_MULTIFY - 1,
		bx, by,
		HMemDC, 0, 0, SRCCOPY);

	if (IntroCnt < 100)
	{
		int temp;
		if ((IntroCnt < 95) && (IntroCnt > 5))
			temp = 2;
		else if ((IntroCnt < 98) && (IntroCnt > 2))
			temp = 1;
		else
			temp = 0;

		hOldIntroBitmap = (HBITMAP)SelectObject(HMemDC, hIntroBitmap);
		bx = IntroBit.bmWidth;
		by = IntroBit.bmHeight;
		TransparentBlt(hdc, (WIN_SIZEX - bx)*0.5, 
			(WIN_SIZEX - by)*0.5,
			bx, by / 3,
			HMemDC, 0, by/3*temp, bx, by / 3,RGB(255,0,255));
	}

	DeleteDC(HMemDC);
}
