#pragma once
class SqUIButton : public UIButton
{
private:
	RECT mRt;
	
	// 비트맵 출력 관련
	int bx, by;
	BITMAP bit, ColtBit;
	HBITMAP hBitmap, hOldBitmap;
	HBITMAP hColtBitmap, hOldColtBitmap;

public:
	bool IsClicked(LPARAM lParam);
	void Draw(HDC hdc, POINT start);
	void DrawColt(HDC hdc, POINT start);
	SqUIButton(LPARAM & gLParam, int & gOnlyMessage, POINT pos, int wide, int hight, int ID);
	virtual ~SqUIButton();
};

