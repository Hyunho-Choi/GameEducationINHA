#pragma once
class FlipAnimation
{
private:
	static HBITMAP hAniImage;
	BITMAP bitAni;
	const int Sprite_Size_X = 10;
	const int Sprite_Size_Y = 10;
	int		cur_Frame = 0;
	bool	delTriger = false;
	ArrPoint mPos = {30,30};
	int mStartCnt;
	
public:

	FlipAnimation();
	~FlipAnimation();
	bool GetDelTriger();
	void DrawBitmap(HDC hdc);
	void DeleteBitmap();
	void UpdateFrame();
};

