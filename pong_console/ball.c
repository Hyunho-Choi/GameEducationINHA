#pragma once
#include <windows.h>
#include <stdio.h>

typedef struct VECTOR {
	float x;
	float y;
}VECTOR;

extern VECTOR VecBall;

extern float ballX;
extern float ballY;
extern int leftBarY;
extern int rightBarY;
extern int leftBarX;
extern int rightBarX;


void ball() {

	
	//공, 벽 충돌

	/*
	if ((ballX == leftBarX || ballX == leftBarX + 1) && (ballY >= leftBarY - 1 && ballY <= leftBarY + 5)) {
		VecBall.x *= -1;
	}
	*/

	if ((ballX <= leftBarX + 1) && (ballY >= leftBarY - 1 && ballY <= leftBarY + 5)) {
		VecBall.x *= -1;
		if (ballY >= leftBarY && ballY <= leftBarY + 4) {
			if (VecBall.x > 0)
				VecBall.x = 2;
			else
				VecBall.x = -2;
		}
		else if (ballY == leftBarY - 1 || ballY == leftBarY + 5) {
			if (VecBall.x > 0)
				VecBall.x = 1;
			else
				VecBall.x = -1;
		}
	}

	if ((ballX >= rightBarX - 1) && (ballY >= rightBarY - 1 && ballY <= rightBarY + 5)) {
		VecBall.x *= -1;
		if (ballY >= rightBarY && ballY <= rightBarY + 4) {
			if(VecBall.x > 0)
				VecBall.x = 2;
			else
				VecBall.x = -2;
		}

		else if (ballY == rightBarY - 1 || ballY == rightBarY + 5) {
			if (VecBall.x > 0)
				VecBall.x = 1;
			else
				VecBall.x = -1;
		}
	}

	/*
	if ((ballX == rightBarX - 2 || ballX == rightBarX - 3) && (ballY >= rightBarY - 1 && ballY <= rightBarY + 5)) {
		VecBall.x *= -1;
	}
	*/

	if (ballY >= 39 || ballY <= 1) {
		VecBall.y *= -1;
	}

	ballX += VecBall.x;
	ballY += VecBall.y;
}