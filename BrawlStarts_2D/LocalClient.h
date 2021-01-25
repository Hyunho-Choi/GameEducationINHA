#pragma once
class LocalClient
{
private:
	HDC mHdc, UIHdc;
	HBITMAP bit, oldbit;
	HBITMAP hBackBitmap, hOldBackBitmap;
	HBITMAP hBackBackBitmap, hOldBackBackBitmap;
	HBITMAP hIntroBitmap, hOldIntroBitmap;
	BITMAP BackBit , BackBackBit, IntroBit;
	ClientMap ClaMap;
	std::vector<Enemy> Players;
	std::vector<Bullet> Bullets;
	int backMargin = 100;
	int IntroCnt = 300;

public:
	PacketServerToClient UserBuffer[2];
	LocalClient();
	virtual ~LocalClient();
	void Draw(HDC hdc);
	void Update();
	void DrawGame(HDC hdc);
	void DrawBackground(HDC hdc);
	void DrawIntro(HDC hdc);
};

