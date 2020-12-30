#include "stdafx.h"
#include "Character.h"


Character::Character()
{
	inTheArea = true;
	this->pos = { MAPSIZE / 2 + 1  , MAPSIZE / 2 + 1 };
}


Character::~Character()
{
}

bool Character::SetPos(ArrPoint movePt)
{
	return false;
}

bool Character::PlusPos(ArrPoint movePt)
{
	if ((pos.ArrY + movePt.ArrY >= 2) && (pos.ArrY + movePt.ArrY < MAPSIZE - 2) &&
		(pos.ArrX + movePt.ArrX >= 2) && (pos.ArrX + movePt.ArrX  < MAPSIZE - 2))
	{
			pos.ArrX += movePt.ArrX;
			pos.ArrY += movePt.ArrY;

			if ((insideMap.GetMap({ pos.ArrX ,pos.ArrY }) == 3) ||
				!(this->IsOnTheLine()))
			{
				pos.ArrX -= movePt.ArrX;
				pos.ArrY -= movePt.ArrY;
				return false;
			}
			return true; //�����ȿ��� ������
	}
	return false;
}

bool Character::Controller(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		this->PlusPos({ 0, -MOVESPEED });
		Escape();
		Update();
		break;
	case VK_UP:
		this->PlusPos({ -MOVESPEED, 0 });
		Escape();
		Update();
		break;
	case VK_RIGHT:
		this->PlusPos({ 0, MOVESPEED });
		Escape();
		Update();
		break;
	case VK_DOWN:
		this->PlusPos({ MOVESPEED, 0 });
		Escape();
		Update();
		break;
	}
	return true;
}

ArrPoint Character::GetPos()
{
	return pos;
}

void Character::Update()
{
	switch (insideMap.GetMap(pos))
	{
	case 0: // �����ȿ��� �������̽�
		inTheArea = false;
		break;
	case 2: // �����ȿ��� ������ ���̽� or ���� �ۿ��� ������ �����̽�
		if (inTheArea == false)
		{
			insideMap.FillOutSide();
			// ���ο����� ������ �� �������� �缳��
		}
		else
		{
			IsOnTheLine();
		}
		inTheArea = true;
		break;
	case 3: // �������� �����̴� ���̽�
		inTheArea = false;
		break;
	}

	if (!inTheArea)
	{
		insideMap.SetMap(pos, 3);
	}
}

void Character::Draw(HDC hdc)
{
	CharBrush = CreateSolidBrush(RGB(196, 136, 130));
	CharPen = CreatePen(PS_INSIDEFRAME | PS_NULL, 0, RGB(196, 136, 130));

	oldPen = (HPEN)SelectObject(hdc, CharPen);
	oldBrush = (HBRUSH)SelectObject(hdc, CharBrush);

	int tempX = (PIXELSIZE*pos.ArrY) + (PIXELSIZE / 2) - (CHARSIZE / 2);
	int tempY = (PIXELSIZE*pos.ArrX) + (PIXELSIZE / 2) - (CHARSIZE / 2);
	//Rectangle(hdc, tempX, tempY, tempX + CHARSIZE, tempY + CHARSIZE);
	
	Ellipse(hdc, tempX , tempY, tempX + CHARSIZE, tempY + CHARSIZE);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);

	DeleteObject(CharBrush);
	DeleteObject(CharPen);
}

bool Character::IsOnTheLine()
{
	//���� �ö����� ��� �ڱ���� �ݴ�� �̵�
	while(!( insideMap.GetMap({ pos.ArrX+1, pos.ArrY+1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX+1, pos.ArrY-1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX+1, pos.ArrY }) != 2 ||
		insideMap.GetMap({ pos.ArrX, pos.ArrY+1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX, pos.ArrY-1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX-1, pos.ArrY+1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX-1, pos.ArrY-1 }) != 2 ||
		insideMap.GetMap({ pos.ArrX-1, pos.ArrY }) != 2))
	{ 
		//���ʿ� ����
		return false;
	}
	return true;
}

void Character::Restart()
{
	inTheArea = true;
	this->pos = { MAPSIZE / 2 + 1  , MAPSIZE / 2 + 1 };
	insideMap.Restart();
}

void Character::Escape()
{
	if ((insideMap.GetMap({ pos.ArrX + 1, pos.ArrY }) == 2) && 
		(insideMap.GetMap({ pos.ArrX - 1, pos.ArrY }) == 2) &&
		(insideMap.GetMap({ pos.ArrX , pos.ArrY + 1 }) == 2) &&
		(insideMap.GetMap({ pos.ArrX , pos.ArrY - 1 }) == 2) &&
		(insideMap.GetMap({ pos.ArrX + 1, pos.ArrY + 1 }) == 2) &&
		(insideMap.GetMap({ pos.ArrX -1, pos.ArrY -1}) == 2) &&
		(insideMap.GetMap({ pos.ArrX +1, pos.ArrY -1 }) == 2) &&
		(insideMap.GetMap({ pos.ArrX -1, pos.ArrY + 1 }) == 2))
	{
		while ((insideMap.GetMap({ pos.ArrX + 1, pos.ArrY }) == 2))
		{
			pos.ArrX++;
		}
	}

}


