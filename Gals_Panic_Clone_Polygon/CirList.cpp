#include "stdafx.h"
#include "CirList.h"
using namespace std;

stPOINT* CirList::GetCloesistPos(stPOINT* A, CirList& List)
{
	stPOINT* result;
	double minDist = 9999;
	int serX = A->value.x;
	int serY = A->value.y;
	int compX, compY;
	
	for (int i = 0; i < List.size(); i++) 
	{
		compX = List.nTimeNextPointer(i)->value.x;
		compY = List.nTimeNextPointer(i)->value.y;
		double compInt = pow(pow(serX - compX, 2) + pow(serY - compY, 2), 0.5);
		if (minDist > compInt)
		{
			result = List.nTimeNextPointer(i);
			minDist = compInt;
		}
	}
	return result;
}

CirList::CirList()
{
	m_size = 0;
	head = nullptr;

	/*
	m_size = 0;
	head = new stPOINT;
	*/
}


CirList::~CirList()
{
	this->reset();
	/*
	stPOINT* del = head;
	for (int i = 0; i < m_size; i++)
	{
		stPOINT* temp = del;
		delete temp;
		if(del->next != nullptr)
			del = del->next;
	}
	*/
}

void CirList::push_back(POINT input)
{
	if (this->head == nullptr)
	{
		head = new stPOINT;
		head->value = input;
		head->next = head;
		head->prev = head;
		m_size++;
	}
	else
	{
		stPOINT* temp = new stPOINT;
		temp->value = input;

		temp->next = head;
		temp->prev = head->prev;
		head->prev->next = temp;
		head->prev = temp;

		m_size++;
	}
	/*
	if (m_size == 0)
	{
		head->value = input;
		head->next = head;
		head->prev = head;
		m_size++;
	}
	else
	{
		stPOINT* temp = new stPOINT;
		temp->value = input;
		stPOINT* pi = head;
		for(int i = 1; i < m_size; i++)
		{
			pi = pi->next;
		}
		temp->next = pi->next;
		temp->prev = pi;
		pi->next->prev = temp;
		pi->next = temp;
		
		m_size++;
	}
	*/
}
int CirList::size()
{
	return m_size;
}
stPOINT * CirList::begin()
{
	return head;
}
stPOINT * CirList::erase(stPOINT * pErase)
{
	if (m_size == 1)
	{
		delete head;
		head = nullptr;
		return nullptr;
	}

	stPOINT * result = pErase->next;

	if (pErase == head)
		head = pErase->prev;
	pErase->prev->next = pErase->next;
	pErase->next->prev = pErase->prev;

	delete pErase;
	m_size--;

	return result;
	/*
	if (pErase == head)
		head = pErase->next;

	stPOINT * result = pErase->next;
	pErase->prev->next = pErase->next;
	pErase->next->prev = pErase->prev;
	delete pErase;
	m_size--;
	return result;
	*/
}
stPOINT * CirList::nTimeNextPointer(int n)
{
	stPOINT * result = head;
	for (int i = 0; i < n; i++)
	{
		result = result->next;
	}
	return result;
}


void CirList::ReformListClockWise(CirList& nowList, CirList& newMakeList)
{
	// 마지막에 헤드 옮김
	stPOINT* startConnectedPt, *endConnectedPt, *delPt;
	
	startConnectedPt = GetCloesistPos(newMakeList.nTimeNextPointer(0), nowList);
	endConnectedPt = GetCloesistPos(newMakeList.nTimeNextPointer(newMakeList.size()-1), nowList);

	delPt = startConnectedPt->next;

	newMakeList.begin()->prev->next = endConnectedPt;
	endConnectedPt->prev = newMakeList.begin()->prev;

	startConnectedPt->next = newMakeList.begin();
	newMakeList.begin()->prev = startConnectedPt;

	for (; delPt != endConnectedPt; )
	{
		stPOINT* temp;
		temp = delPt;
		delPt = delPt->next;
		delete temp;
		m_size--;
	}
	nowList.head=endConnectedPt;
	m_size += (newMakeList.size());
	newMakeList.notDeleteReset();
}

void CirList::ReformListAntiClockWise(CirList & nowList, CirList & newMakeList)
{
	CirList AnitList;
	newMakeList.reverse(AnitList);
	newMakeList.reset();

	nowList.ReformListClockWise(nowList, AnitList);
}

double CirList::GetArea()
{
	double result = 0;
	int i = 0;
	for (stPOINT* it = head ; i < m_size; i++)
	{
		result += (it->value.x * it->next->value.y) -
			(it->next->value.x * it->value.y);
		it = it->next;
	}

	result = result < 0 ? -result : result;

	return 0.5*result;
}

void CirList::reset()
{

	int nTimes = m_size;
	for (int i = 0; i < nTimes; i++)
	{
		erase(head);
	}
	m_size = 0;
	/*
	this->begin();

	m_size = 0;
	stPOINT* temp = new stPOINT;
	head = temp;
	*/
}

void CirList::reverse(CirList& revList)
{

	stPOINT* temp = this->begin()->prev;
	this->head = temp;
	while (1)
	{
		revList.push_back(temp->value);
		temp = temp->prev;
		if (temp == head)
			break;
	}
	return;

}

void CirList::copy(POINT * pointDraw)
{
	stPOINT* temp = head;
	for (int i = 0; i < m_size; i++)
	{
		pointDraw[i] = temp->value;
		temp = temp->next;
	}
}

void CirList::duplicate(CirList & newList)
{
	newList.reset();
	stPOINT* temp = head;
	for (int i = 0; i < this->size(); i++)
	{
		newList.push_back(temp->value);
		temp = temp->next;
	}
}

void CirList::notDeleteReset()
{
	this->head = nullptr;
	m_size = 0;
}


