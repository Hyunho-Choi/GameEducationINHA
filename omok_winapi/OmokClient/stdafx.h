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
#include <WinSock2.h>
#include <iostream>

// TODO: reference additional headers your program requires here
#include "Function.h"
#include "Struct.h"
/*
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
*/

#pragma comment(lib,"ws2_32.lib")

#define CELL_PITCH 40
#define HALF_CELL_PITCH 20
#define CELL_NUM 18