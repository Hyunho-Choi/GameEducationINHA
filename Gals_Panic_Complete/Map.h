#pragma once
#include <array>


class Map
{
private:	
	static short map[MAPSIZE][MAPSIZE];
	HBRUSH oldBrush, myAreaBrush, newLineBrush, charaBrush, blankBrush;
	HPEN oldPen, myAreaPen, newLinePen, blankPen;
	RECT MaxRect;
	
	
public:
	Map();
	~Map();
	bool InsideScanner3Flag;
	bool SetMap(ArrPoint positon, short val); // 외부 콜 -> 외부콜 할때마다 내부 업데이트
	short GetMap(ArrPoint positon);
	void Update();
	void Draw(HDC hdc);
	/*
	bool InsideScanner(ArrPoint* scan);
	bool InsideScanner2(ArrPoint* scan , int cnt);
	bool InsideScanner3(ArrPoint* scan);
	*/
	bool FillOutSide();
	void FloodFillLast(ArrPoint start);
	void FillNotFive(RECT& Rt, int change, int not);
	//void FloodFill(ArrPoint change, int scan);
	void changeNewLine(ArrPoint start);
	void Reset4to2();
	void ResetAtoB(short A, short B);
	//bool FloodFill2(ArrPoint change, int scan);
	//bool ZeroCase(ArrPoint* scan);
	//void ZeroFlood(ArrPoint change, int scan);
	void Restart();

	


};