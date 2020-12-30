#include "stdafx.h"
#include "Character.h"



Character::Character()
{
	DirAngle = PI*0.5;
	
	mySpeed = { (long)(BASE_SPEED*cos(DirAngle)), (long)(BASE_SPEED*sin(DirAngle)) }; //스피드는 고정
	myPos = { (long)(GAME_AREA * 0.5) , (long)(GAME_AREA * 0.5) };
	lastPos = myPos;
	test = { (long)(GAME_AREA * 0.5 + 100) , (long)(GAME_AREA * 0.5 + 100) };
	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 10, (long)(GAME_AREA * 0.5) - 10 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 5, (long)(GAME_AREA * 0.5) - 10 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5), (long)(GAME_AREA * 0.5) - 10 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 5, (long)(GAME_AREA * 0.5) - 10 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 10, (long)(GAME_AREA * 0.5) - 10 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 10, (long)(GAME_AREA * 0.5) - 5 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 10, (long)(GAME_AREA * 0.5)  });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 10, (long)(GAME_AREA * 0.5) + 5 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 10, (long)(GAME_AREA * 0.5) + 10 });
	
	nowArea.push_back({ (long)(GAME_AREA * 0.5) + 5, (long)(GAME_AREA * 0.5) + 10 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) , (long)(GAME_AREA * 0.5) + 10 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 5, (long)(GAME_AREA * 0.5) + 10 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 10, (long)(GAME_AREA * 0.5) + 10 });

	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 10, (long)(GAME_AREA * 0.5) + 5 });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 10, (long)(GAME_AREA * 0.5)  });
	nowArea.push_back({ (long)(GAME_AREA * 0.5) - 10, (long)(GAME_AREA * 0.5) - 5 });
	onMyArea = true;
}


Character::~Character()
{
	
}

void Character::Update()
{
	// 위치 업데이트
	lastPos = { myPos.x, myPos.y };

	myPos.x += mySpeed.x;
	myPos.y += mySpeed.y;

	if ((onMyArea == true) && (IsPointInPolygon(myPos, nowArea) == false))
		newStartPos = { lastPos.x, lastPos.y };

	if ((onMyArea == false) && (IsPointInPolygon(myPos, nowArea) == true))
		newLastPos = { myPos.x, myPos.y };

	// 영역 안밖 판정
	if(nowArea.size())
		onMyArea = IsPointInPolygon(myPos, nowArea);


	if (!onMyArea) // 영역 밖
	{
		// 위치 편입

		bool isFind = false;
		stPOINT* forTemp = nowArea.begin();
		for (int i = 0; i < nowArea.size() ; i++)
		{ 
			if ((myPos.x == forTemp->value.x) && (myPos.y == forTemp->value.y))
				isFind = true;
			forTemp = forTemp->next;
		}
		//nowIt = find(nowArea.begin(), nowArea.end(), myPos);

		if ((!isFind) || (nowArea.size() == 0))
		{
			// 못찾음
			// 임시로 현재위치에 편입
			newMakeArea.push_back(myPos);




			
			//if (lastPos.x == 0 && lastPos.y == 0)
			//	newStartPos = { myPos.x, myPos.y }; 
		}
		else
		{
			// 찾음
		}
	}
	else //영역 안
	{
		//newLastPos = lastPos;
		//newArea 편입 (0이면 자동으로 안함)
		if (newMakeArea.size() != 0)
		{
			CirList dupl1, dupl2; //소멸자 오류
			CirList duplMake1, duplMake2;
			
			nowArea.duplicate(dupl1);
			nowArea.duplicate(dupl2);
			newMakeArea.duplicate(duplMake1);
			newMakeArea.duplicate(duplMake2);

			dupl1.ReformListClockWise(dupl1, duplMake1);
			dupl2.ReformListAntiClockWise(dupl2, duplMake2);

			if(dupl1.GetArea() > dupl2.GetArea())
				nowArea.ReformListClockWise(nowArea, newMakeArea);
			else
				nowArea.ReformListAntiClockWise(nowArea, newMakeArea);
		}
			








		//lastPos 리셋
		lastPos = {0,0};
	}
	
	// IsPointInPolygon함수 테스트
	
	if (nowArea.size() > 10)
	{
		testLight = IsPointInPolygon(test, nowArea);
	}
		
	

}

void Character::Draw(HDC hdc)
{
	POINT* pointDraw = new POINT[nowArea.size()];
	nowArea.copy(pointDraw);
	POINT* newpointDraw = new POINT[newMakeArea.size()];
	newMakeArea.copy(newpointDraw);
	HPEN OldPen;

	HBRUSH MyAreaBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, MyAreaBrush);
	HPEN NewAreaPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));

	OldPen = (HPEN)SelectObject(hdc, NewAreaPen);
	Polygon(hdc, newpointDraw, newMakeArea.size());
	SelectObject(hdc, OldBrush);

	SelectObject(hdc, OldPen);
	
	HBRUSH NewAreaBrush = CreateSolidBrush(RGB(255, 0, 255));
	
	
	OldBrush = (HBRUSH)SelectObject(hdc, NewAreaBrush);
	
	Polygon(hdc, pointDraw, nowArea.size());
	SelectObject(hdc, OldBrush);

	Ellipse(hdc, myPos.x - 5, myPos.y - 5, myPos.x + 5, myPos.y + 5);
	/*
	if (testLight)
	{
		HBRUSH MyAreaBrush2 = CreateSolidBrush(RGB(255, 0, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyAreaBrush);
		Ellipse(hdc, test.x - 5, test.y - 5, test.x + 5, test.y + 5);
		SelectObject(hdc, OldBrush);
	}
	else
	{
		Ellipse(hdc, test.x - 5, test.y - 5, test.x + 5, test.y + 5);
	}
	*/


	delete[] pointDraw;
}

void Character::Controllor()
{
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		howManyturnRight++;
		DirAngle += ROTATE_LEVEL;
		if (DirAngle > 2 * PI)
			DirAngle - 2 * PI;
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		howManyturnRight--;
		DirAngle -= ROTATE_LEVEL;
		if (DirAngle < -2 * PI)
			DirAngle + 2 * PI;
	}
	mySpeed = { (long)(BASE_SPEED*cos(DirAngle)), (long)(BASE_SPEED*sin(DirAngle)) };
}


bool Character::IsPointInPolygon(static POINT point, static CirList& polygon)
{
	int polygonLength = polygon.size(), i = 0;
	bool inside = false;
	float pointX = point.x, pointY = point.y;
	float startX, startY, endX, endY;
	stPOINT* it = polygon.begin()->prev;
	stPOINT endPoint = *it;
	endX = endPoint.value.x;
	endY = endPoint.value.y;
	it = polygon.begin();

	while (i < polygonLength)
	{
		it = it->next;
		startX = endX; startY = endY;
		endPoint = *it;
		i++;
		
		endX = endPoint.value.x; endY = endPoint.value.y;
		inside ^= (endY > pointY ^ startY > pointY) && ((pointX - endX) < (pointY - endY) * (startX - endX) / (startY - endY));
	}
	return inside;
}


/*
void Character::MergeNewMake(CirList& nowArea, CirList& newMakeArea)
{
	if (newMakeArea.size() < 4)
		return;

	list<POINT>::iterator forIt = nowArea.begin();

	for (; forIt != nowArea.end(); forIt++)
	{
		if (forIt == nowArea.end())
			forIt = nowArea.begin();
			
		if (GetDistance((*forIt), newStartPos) <= BASE_SPEED*2)
			break;
	}
	//forIt++;
	int newmakesize = newMakeArea.size();
	nowArea.splice(forIt, newMakeArea);
	
	for (int i = 0; i < newmakesize; i++)
	{
		forIt++;
	}
	
	while (1)
	{
		if (forIt == nowArea.end())
			forIt = nowArea.begin();
		if (GetDistance((*forIt), newLastPos) > (BASE_SPEED * 2))
			forIt = nowArea.erase(forIt);
		else
			break;
	}
}


void Character::MergeNewMake2(CirList& nowArea, CirList& newMakeArea)
{
	if (newMakeArea.size() < 4)
		return;

	list<POINT>::iterator temp = GetClosistPoint(nowArea, newStartPos);

	if (howManyturnRight > 0)
	{
		list<POINT>::iterator erase = GetClosistPoint(nowArea, newStartPos);
		list<POINT>::iterator eraseEnd = GetClosistPoint(nowArea, newLastPos);
		while (erase != eraseEnd) // 정방향
		{
			if (erase == nowArea.end())
				erase = nowArea.begin();

			erase = nowArea.erase(erase);
		}
	}
	else
	{
		list<POINT>::iterator erase = GetClosistPoint(nowArea, newStartPos);
		list<POINT>::iterator eraseEnd = GetClosistPoint(nowArea, newLastPos);
		while (erase != eraseEnd) // 정방향
		{
			if (eraseEnd == nowArea.end())
				eraseEnd = nowArea.begin();

			eraseEnd = nowArea.erase(eraseEnd);
		}
	}

	nowArea.splice(temp, newMakeArea);

}
*/



double Character::GetDistance(POINT A, POINT B)
{
	return(pow(pow(A.x - B.x, 2) + pow(A.y - B.y, 2), 0.5));
}