#include "stdafx.h"
#include "UIConversation.h"


UIConversation::UIConversation()
	:UIImage("Resources/UI/Setting/Conver2.png", { 0, 0 }, 64, 36)
	, OutputText("")
{

	D3DXCreateFont(
		gD3Device,               //그래픽 카드 인터페이스
		50,                              // 글자 세로 크기
		25,                               // 글자 가로 크기
		500,                            // 굵기 0~1000사이
		1,                                // 밉맵과 비슷하게 사용되는 것 . 1로 준다
		false,                           // 이텔릭 ( 기울기 ) 
		DEFAULT_CHARSET,       // 일단 이값으로...
		OUT_DEFAULT_PRECIS,   // 정밀도
		DEFAULT_QUALITY,        // 일단 이값으로...
		0,                                // 일단 이값으로...
		L"신명조",                      // 사용 글꼴
		&mFont);
}

UIConversation::~UIConversation()
{
}

void UIConversation::SetConversation(string newText)
{
	this->SetVisible(true);
	OutputText = newText;
}

void UIConversation::Render()
{
	if (mbVisible)
	{
		UIImage::Render();
		RECT rc = { 100, 650, -1, -1 };
		mFont->DrawTextA(NULL, OutputText.c_str(), -1, &rc, DT_LEFT | DT_NOCLIP, 0xff000000);
	}
}


