#pragma once
class ServerMap
{
private:
	MapTile Map[MAPSIZEY][MAPSIZEX];

public:
	void SetMap();
	void SendMap();
	int GetMapState(int x, int y);
	RECT* GetMapRt(int x, int y);
	void ImportMap();
	ServerMap();
	~ServerMap();
};

