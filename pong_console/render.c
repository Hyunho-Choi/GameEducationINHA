#pragma once
#include <windows.h>
#include <stdio.h>


extern int leftBarY;
extern int rightBarY;

extern float ballX;
extern float ballY;

extern int rightScore;
extern int leftScore;
extern int leftBarX;
extern int rightBarX;



void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void render() {

	wchar_t* bar = L"█";

	//왼쪽 바 출력
	gotoxy(leftBarX, leftBarY);
	puts("■");
	gotoxy(leftBarX, leftBarY + 1);
	puts("■");
	gotoxy(leftBarX, leftBarY + 2);
	puts("■");
	gotoxy(leftBarX, leftBarY + 3);
	puts("■");
	gotoxy(leftBarX, leftBarY + 4);
	puts("■");

	//오른쪽 바 출력
	gotoxy(rightBarX, rightBarY);
	puts("■");
	gotoxy(rightBarX, rightBarY + 1);
	puts("■");
	gotoxy(rightBarX, rightBarY + 2);
	puts("■");
	gotoxy(rightBarX, rightBarY + 3);
	puts("■"); 
	gotoxy(rightBarX, rightBarY + 4);
	puts("■");

	//공 출력
	gotoxy((int)ballX, (int)ballY);
	printf("●");

	//점수 출력
	gotoxy(50, 8);
	printf("%d",leftScore);
	gotoxy(100, 8);
	printf("%d",rightScore);

}


