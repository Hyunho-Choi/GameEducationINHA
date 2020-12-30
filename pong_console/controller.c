#pragma once
#include <windows.h>
#include <stdio.h>


extern int leftBarY;
extern int rightBarY;

void controller() {

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		leftBarY--;
		if (leftBarY == -1)
			leftBarY++;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		leftBarY++;
		if (leftBarY == 35)
			leftBarY--;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		rightBarY--;
		if (rightBarY == -1)
			rightBarY++;
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		rightBarY++;
		if (rightBarY == 35)
			rightBarY--;
	}


}