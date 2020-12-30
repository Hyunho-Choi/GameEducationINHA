#pragma once
#include <Windows.h>
#include <cmath>


struct stPOINT
{
	stPOINT* prev = nullptr;
	stPOINT* next = nullptr;
	POINT value;
};

class CirList
{
private:
	stPOINT* head;
	int m_size;

	stPOINT* GetCloesistPos(stPOINT* A, CirList& List);

public:
	CirList();
	~CirList();
	void push_back(POINT input);
	int size(); 
	stPOINT* begin();
	stPOINT* erase(stPOINT* pErase);
	stPOINT* nTimeNextPointer(int n);
	void ReformListClockWise(CirList& nowList, CirList& newMakeList);
	void ReformListAntiClockWise(CirList& nowList, CirList& newMakeList);
	double GetArea();
	void reset();
	void reverse(CirList& revList);
	void copy(POINT* pointDraw);
	void duplicate(CirList& newList);
	void notDeleteReset();
};

