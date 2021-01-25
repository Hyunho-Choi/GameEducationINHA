#include "stdafx.h"
//#include "UIButton.h"

POINT UIButton::GetmPos()
{
	return mPos;
}

bool UIButton::SetmPos(POINT input)
{
	if ((input.x < 0) || (input.y < 0))
	{
		return false;
	}
	else
	{
		mPos = input;
		return true;
	}
}

void UIButton::Update()
{
	if (this->IsClicked(*mpLParam))
		*mOlnyMessage = mID;
}

UIButton::UIButton(LPARAM & gLParam, int & gOnlyMessage, POINT pos, int ID)
{
	mID = ID;
	mpLParam = &gLParam;
	mPos = pos;
	if (gOnlyMessage == NULL)
	{
		mOlnyMessage = &gOnlyMessage;
	}
	else
	{

	}

}

UIButton::~UIButton()
{
}
