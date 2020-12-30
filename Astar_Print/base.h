#pragma once

#define DEFALT_DIST 10
#define DEFALT_DIST_CROSS 14
#define MAP_SLICE 15
#define INF_WIGHT 999

struct TILE
{
	int wight = 1;
	bool able = true;
	bool closed = false;
	bool opened = false;
	int F = 0;
	int H = -1;
	int G = -1;
};
