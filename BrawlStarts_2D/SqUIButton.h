#pragma once
class SqUIButton : public UIButton
{
private:
	RECT mRt;
	
	// ��Ʈ�� ��� ����
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

