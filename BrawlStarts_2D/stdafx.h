// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>



// TODO: reference additional headers your program requires here


enum BtnMsg {
	PLAY = 101,
	GAME_MODE,
	MY_MENU,
	BLUE_BOX,
	PURPLE_BOX,
	CHARACTER,
	SOCIAL,
	SHOP,
	BRAWLER
};

enum UserState {
	IDLE = 200,
	MOVE,
	FIRE,
	DEMEGED
};

// 맵상태가 300부터 (350부터 충돌 가능맵
enum MapState {
	BASIC = 300,
	BUSH = 301,
	OUTOFGAME = 350,
	ROCK = 360,
	ITMEBOX,
	CACTUS,
	NOTILE,
	STUMP 
};


enum GameMode {
	LOBBY = 400,
	INGAME,
	ONLINELOBBY
};

enum IntDir {
	UP = 500,
	DOWN,
	LEFT,
	RIGHT,
};


//void ErrorMessage(char * strMessage);

#define WIN_1080 1080
#define WIN_1920 1920
#define WIN_720 779
#define WIN_1280 1296
#define WIN_SIZEX WIN_1280 
#define WIN_SIZEY WIN_720
#define PI 3.14159265
#define CHAR_R 50.
#define BULLET_W 40.
#define BULLET_THETA atan(BULLET_W/CHAR_R)
#define MOVE_ONE_TICK 1
#define MAPTILE_SIZE 10
#define GAMETILE_SIZE 30
#define GAME_MULTIFY 5
#define MAPSIZEX 24
#define MAPSIZEY 44
#define XMARGIN (WIN_SIZEX - MAPTILE_SIZE * GAME_MULTIFY * MAPSIZEX) * 0.5
#define TILE_MARGIN 10
#define BULLET_CNT 18


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
//#include "ServerMap.h"
#include "resource.h"
#include "Bullet.h"
#include "Struct.h"
#include "UIButton.h"
#include "SqUIButton.h"
#include "UIManager.h"
#include "Enemy.h"
#include "ClientMap.h"
#include "ServerMap.h"
#include "LocalClient.h"
#include "LocalServer.h"
#include "UserController.h"



#pragma comment(lib, "msimg32.lib")



