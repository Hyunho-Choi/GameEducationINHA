#pragma once
class ClientMap
{
private:
	MapTile Map[MAPSIZEY][MAPSIZEX];
	HBITMAP hBitmap, hOldBitmap;
	BITMAP bit;


public:
	// draw
	void draw(HDC hdc);
	// 서버로부터 받을 방법
	void DrawGame(HDC hdc, int y);
	void ImportMap();
	ClientMap();
	~ClientMap();
};

