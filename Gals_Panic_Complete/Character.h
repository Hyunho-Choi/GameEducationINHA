#pragma once
class Character
{
private:
	ArrPoint pos;
	bool inTheArea;
	const int size = PIXELSIZE + 3;
	Map insideMap;
	ArrPoint start;
	HBRUSH oldBrush, CharBrush;
	HPEN oldPen, CharPen;

public:
	Character();
	~Character();
	bool SetPos(ArrPoint movePt);
	bool PlusPos(ArrPoint movePt);
	bool Controller(WPARAM wParam);
	ArrPoint GetPos();
	void Update();
	void Draw(HDC hdc);
	bool IsOnTheLine();
	void Restart();
	void Escape();
};

