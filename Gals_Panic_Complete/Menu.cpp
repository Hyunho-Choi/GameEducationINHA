#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{

}


Menu::~Menu()
{

}

bool Menu::IsStartState()
{
	return StartState;
}

bool Menu::IsEndState()
{
	return EndState;
}

void Menu::SetStartState(bool in)
{
	StartState = in;
}

void Menu::SetEndState(bool in)
{
	EndState = in;
}

void Menu::StartDraw(HDC hdc)
{
	
	HFONT font, oldFont;
	font = CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, nullptr);
	oldFont = (HFONT)SelectObject(hdc, font);


	Rectangle(hdc, TitleButton.left, TitleButton.top, TitleButton.right, TitleButton.bottom);
	DrawText(hdc, L"T I T L E", -1, &TitleButton, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	Rectangle(hdc, StartButton.left, StartButton.top, StartButton.right, StartButton.bottom);
	DrawText(hdc, L"START", -1, &StartButton, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	SelectObject(hdc, oldFont);
	DeleteObject(font);
}

void Menu::EndDraw(HDC hdc)
{
	HFONT font, oldFont;
	font = CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, nullptr);
	oldFont = (HFONT)SelectObject(hdc, font);


	Rectangle(hdc, TitleButton.left, TitleButton.top, TitleButton.right, TitleButton.bottom);
	DrawText(hdc, L"GAME OVER", -1, &TitleButton, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	Rectangle(hdc, StartButton.left, StartButton.top, StartButton.right, StartButton.bottom);
	DrawText(hdc, L"RESET", -1, &StartButton, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	SelectObject(hdc, oldFont);
	DeleteObject(font);
}

void Menu::PutStartClick(LPARAM lParam)
{
	POINT mousePos;
	mousePos.x = LOWORD(lParam);
	mousePos.y = HIWORD(lParam);

	if (PtInRect(&StartButton, mousePos))
		StartState = false;
}

bool Menu::PutEndClick(LPARAM lParam)
{
	POINT mousePos;
	mousePos.x = LOWORD(lParam);
	mousePos.y = HIWORD(lParam);

	if (PtInRect(&StartButton, mousePos))
	{
		EndState = false;
		return true;
	}
	return false;
}
