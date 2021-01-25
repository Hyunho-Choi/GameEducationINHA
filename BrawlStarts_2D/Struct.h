#pragma once
#include "stdafx.h"

struct PacketServerToClient
{
	bool isNew;
	POINT pos;
	int direction;
	int IntDir;
	int life;
	int state; // 대기, 이동, 공격, 피격 200부터 배정
	float bulletCnt = 3;
	Bullet myBullet[BULLET_CNT];
};

struct MapTile
{
	RECT rt;
	int state = 0;
};

struct PacketControllerToServer
{
	bool isNew;
	bool isClick;
	int ConIntDir;
	POINT Move;
	LPARAM mouseLparam;
};