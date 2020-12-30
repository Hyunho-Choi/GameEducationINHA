#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define line 40
#define col 150

typedef struct VECTOR {
	float x;
	float y;
}VECTOR;

int leftBarY = line / 2;
int rightBarY = line / 2;

int leftBarX = 3;
int rightBarX = 145;

int rightScore = 0;
int leftScore = 0;

float ballX = col / 2;
float ballY = line / 2;


int x2mm = 144;
int x1pp = 4;
VECTOR VecBall =  { 2,1 };

void main(){

	system("mode con:cols=150 lines=40");
	CursorView2(0);
	DWORD dwOldTime = 0;
	DWORD dwCurrentTime = 0;
	int menu = 0;

	dwCurrentTime = dwOldTime = time(NULL);
	float fDeltaTime = 0;
	float fAccumulationTime = 0;

	while (1) {

		dwCurrentTime = clock();
		fDeltaTime = (float)(dwCurrentTime - dwOldTime) / 1000;
		dwOldTime = dwCurrentTime;
		fAccumulationTime += fDeltaTime;

		if (fAccumulationTime > 0.036f)
		{
			fAccumulationTime = 0;

			if (ballX < leftBarX-1) {
				rightScore++;
				ballX = col / 2;
				ballY = line / 2;
				system("cls");
				render();
				if (VecBall.x > 0)
					VecBall.x = 2;
				else
					VecBall.x = -2;
				Sleep(1000);
				if (rightScore == 3)
					break;
			}

			if (ballX > rightBarX+1) {
				leftScore++;
				ballX = col / 2;
				ballY = line / 2;
				system("cls");
				render();
				if (VecBall.x > 0)
					VecBall.x = 2;
				else
					VecBall.x = -2;
				Sleep(1000);
				if (leftScore == 3)
					break;
			}
			system("cls");
			render();
			ball();
			controller();
		}
		
	}
	gotoxy(75, 20);
	printf("Á¾·á");
	Sleep(3000);
	system("pause");
	return 0;
}


