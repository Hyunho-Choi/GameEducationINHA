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
	// �����κ��� ���� ���
	void DrawGame(HDC hdc, int y);
	void ImportMap();
	ClientMap();
	~ClientMap();
};

