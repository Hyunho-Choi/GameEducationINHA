#include "stdafx.h"
//#include "SqUIButton.h"

bool SqUIButton::IsClicked(LPARAM lParam)
{
	if ((LOWORD(lParam) > mRt.left) && (LOWORD(lParam) < mRt.right) && (HIWORD(lParam) > mRt.top) && (HIWORD(lParam) < mRt.bottom))
		return true;
	else
		return false;
}

void SqUIButton::Draw(HDC hdc, POINT start)
{
	HDC HMemDC = CreateCompatibleDC(hdc);

	//Rectangle(hdc, mRt.left, mRt.top, mRt.right, mRt.bottom);
	
	hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	TransparentBlt(hdc, mRt.left, mRt.top, mRt.right- mRt.left,mRt.bottom - mRt.top, 
		HMemDC, start.x, start.y, mRt.right - mRt.left, mRt.bottom - mRt.top, RGB(255,0,255));

	SelectObject(HMemDC, hOldBitmap);
	DeleteDC(HMemDC);

}

void SqUIButton::DrawColt(HDC hdc, POINT start)
{
	HDC HMemDC = CreateCompatibleDC(hdc);

	//Rectangle(hdc, mRt.left, mRt.top, mRt.right, mRt.bottom);
	hOldColtBitmap = (HBITMAP)SelectObject(HMemDC, hColtBitmap);
	bx = ColtBit.bmWidth;
	by = ColtBit.bmHeight;

	static int aniX, aniY = 0;

	TransparentBlt(hdc, mRt.left, mRt.top, mRt.right - mRt.left, mRt.bottom - mRt.top,
		HMemDC, aniX, aniY, (int)(bx*0.1), by, RGB(255, 0, 255));

	aniX += (int)(bx*0.1);
	if (aniX >= bx)
		aniX = 0;

	SelectObject(HMemDC, hOldColtBitmap);
	DeleteDC(HMemDC);
}

SqUIButton::SqUIButton(LPARAM & gLParam, int & gOnlyMessage, POINT pos , int wide, int hight, int ID)
	: UIButton(gLParam, gOnlyMessage, pos, ID)
{
	mRt = { pos.x - (wide / 2) , pos.y - (hight / 2) , pos.x + (wide / 2) , pos.y + (hight / 2) }; //ltrb
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/purple_box2.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBitmap, sizeof(BITMAP), &bit);
	hColtBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/lobby_colt.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hColtBitmap, sizeof(BITMAP), &ColtBit);

}

SqUIButton::~SqUIButton()
{
	DeleteObject(hBitmap);
}
