#pragma once

#define MAPSIZE 80
#define PIXELSIZE 10
#define MOVESPEED 1
#define CHARSIZE 10
#define DEFALTMONSTER 10


//0 ��ĭ 1 �� ������ 2 ������ 3 ���ο��=���� 4 �ӽ������ ���� 5 �����ִ�ܺζ���

enum DIR {Dir_Right, Dir_Up, Dir_Left, Dir_Down};

using namespace std;

typedef struct {
	long ArrX;
	long ArrY;
}ArrPoint; 

void CreateBitmap(HBITMAP& hBackImage, BITMAP& bitBack);
void DrawBitmap(HWND hWnd, HDC hdc, HBITMAP& hBackImage, BITMAP& bitBack);
void DeleteBitmap(HBITMAP& hBackImage);
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT rtView);

/*
void CreateBitmap(HBITMAP hBackImage , BITMAP bitBack)
{
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("image.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage, sizeof(BITMAP), &bitBack);
}

void DrawBitmap(HWND hWnd, HDC hdc, HBITMAP hBackImage, BITMAP bitBack)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
}

void DeleteBitmap(HBITMAP hBackImage)
{
	DeleteObject(hBackImage);
}
*/