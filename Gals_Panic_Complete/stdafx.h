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
#include <deque>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdlib.h>
#include <crtdbg.h>

// TODO: reference additional headers your program requires here
#include "base.h"
#include "Map.h"
#include "Character.h"
#include "Controller.h"
#include "Monster.h"
#include "Menu.h"

#pragma comment(lib, "msimg32.lib")