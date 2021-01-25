#pragma once
class UIButton
{
private:
	POINT mPos;
	LPARAM* mpLParam;
	int* mOlnyMessage;
	int mID;

public:
	POINT GetmPos();
	bool SetmPos(POINT input);
	virtual bool IsClicked(LPARAM lParam ) = 0;
	virtual void Draw(HDC hdc, POINT start) = 0;
	virtual void DrawColt(HDC hdc, POINT start) = 0;
	void Update();
	UIButton(LPARAM & gLParam, int & gOnlyMessage, POINT pos, int ID);
	virtual ~UIButton();
};

