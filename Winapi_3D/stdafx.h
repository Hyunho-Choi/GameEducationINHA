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
#include <complex.h>

#define E 0.00001
#define PI 3.141592f
#define RAD(x) (x * PI / 180)
#define WIN_WIDE 1080
#define WIN_HIGHT 720
#define FOV PI / 4.0f


#define MOUSE_SENSITIVITY 0.1f


enum SHAPE
{
	GRID = 111,
	CUBE = 444,
	TETRAHEDRON = 333
	
};

enum XYZ
{
	enumX = 100,
	enumY,
	enumZ
};

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// TODO: reference additional headers your program requires here
#include "cVector3.h"
#include "cMatrix.h"
#include "Object.h"


extern HWND g_hWnd;




