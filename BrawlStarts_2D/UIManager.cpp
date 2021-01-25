#include "stdafx.h"
#include "UIManager.h"


void UIManager::CreateUI()
{
	// 기능 구현 버튼
	SqUIButton* pPlayButton = new SqUIButton
	(mLParam, mBtnMsg, { 1100 ,538 + 85 }, 335, 170, PLAY);
	mMenuBtn.push_back(pPlayButton);

	SqUIButton* pGameModeButton = new SqUIButton
	(mLParam, mBtnMsg, { 6 + 200 + 200 + 9 + 41 + 230 ,538 + 85 }, 485, 170, GAME_MODE);
	mMenuBtn.push_back(pGameModeButton);

	SqUIButton* pBlueBoxButton = new SqUIButton
	(mLParam, mBtnMsg, { 6 + 100 + 200 + 5 ,538 + 90 }, 200, 170, BLUE_BOX);
	mMenuBtn.push_back(pBlueBoxButton);

	SqUIButton* pPurpleBoxButton = new SqUIButton
	(mLParam, mBtnMsg, { 6+100,538+85 }, 200, 170, PURPLE_BOX);
	mMenuBtn.push_back(pPurpleBoxButton);

	SqUIButton* pCharaterButtonLabel = new SqUIButton // 캐릭터 라벨
	(mLParam, mBtnMsg, { 666,97 } , 222,76 , CHARACTER);
	mMenuBtn.push_back(pCharaterButtonLabel);

	SqUIButton* pCharaterButton = new SqUIButton  // 캐릭터 출력
	(mLParam, mBtnMsg, { 655, 350 }, 323,440, CHARACTER);
	mMenuBtn.push_back(pCharaterButton);

	SqUIButton* pMyMenuButton = new SqUIButton
	(mLParam, mBtnMsg, { 200,50 }, 400, 99, MY_MENU);
	mMenuBtn.push_back(pMyMenuButton);
	// 기능 미구현 버튼

	SqUIButton* pBrawlerButton = new SqUIButton
	(mLParam, mBtnMsg, { 70,222 }, 120, 85, BRAWLER);
	mMenuBtn.push_back(pBrawlerButton);

	SqUIButton* pShopButton = new SqUIButton
	(mLParam, mBtnMsg, { 70,327 }, 115, 85, SHOP);
	mMenuBtn.push_back(pShopButton);

	SqUIButton* pSocialButton = new SqUIButton
	(mLParam, mBtnMsg, { 70,430 }, 120, 85, SOCIAL);
	mMenuBtn.push_back(pSocialButton);

	SqUIButton* pDrawerButton= new SqUIButton
	(mLParam, mBtnMsg, { 1180,37 }, 180, 60, SHOP);
	mMenuBtn.push_back(pDrawerButton);
}

UIManager::UIManager()
{
	CreateUI();
	// 버튼 설계
}


UIManager::~UIManager()
{
	// 버튼 동적할당 전부 해제
	for (std::vector<UIButton*>::iterator it = mMenuBtn.begin(); it != mMenuBtn.end(); it++)
	{
		delete (*it);
	}
	


	DeleteDC(UIHdc);
}

void UIManager::Draw(HDC hdc)
{
	UIHdc = CreateCompatibleDC(hdc);
	HBITMAP bit, oldbit;
	bit = CreateCompatibleBitmap(hdc, WIN_SIZEX, WIN_SIZEY);
	oldbit = (HBITMAP)SelectObject(UIHdc, bit);

	Rectangle(UIHdc, 0, 0, WIN_SIZEX, WIN_SIZEY); //대충배경
	this->DrawBackground(UIHdc);

	mMenuBtn[0]->Draw(UIHdc, { 685,0 }); // 스타트
	mMenuBtn[1]->Draw(UIHdc, { 200,0 });  // 게임모드
	mMenuBtn[2]->Draw(UIHdc, { 0,170 });  // 블루박스
	mMenuBtn[3]->Draw(UIHdc, { 0,0 });  // 퍼플박스
	
	mMenuBtn[4]->Draw(UIHdc, { 601,261 });  // 캐릭터 라벨
	mMenuBtn[5]->DrawColt(UIHdc, { 0,0 });    //콜트

	mMenuBtn[6]->Draw(UIHdc, { 201,171 });  //마이 메뉴
	mMenuBtn[7]->Draw(UIHdc, { 601,171 });
	mMenuBtn[8]->Draw(UIHdc, { 722,171 });
	mMenuBtn[9]->Draw(UIHdc, { 837,171 });
	mMenuBtn[10]->Draw(UIHdc, { 830,268 });

	//BitBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, UIHdc, 0, 0, SRCCOPY);
	TransparentBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, UIHdc, 0, 0, WIN_SIZEX, WIN_SIZEY, RGB(255, 0, 255));
	//StretchBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, UIHdc, 0, 0, WIN_SIZEX, WIN_SIZEY, SRCCOPY);

	SelectObject(UIHdc, oldbit);
	DeleteObject(bit);
}

bool UIManager::InputMouseClick(LPARAM lParam)
{
	mLParam = lParam;
	for (std::vector<UIButton*>::iterator it = mMenuBtn.begin(); it != mMenuBtn.end(); it++)
	{
		(*it)->Update();
	}
	
	if (PrintConsoleClicked())
		return false;
		
	switch (mBtnMsg)
	{
	case PLAY:
		break;
	case GAME_MODE:
		break;
	case MY_MENU:
		break;
	case BLUE_BOX:
		break;
	case PURPLE_BOX:
		break;
	case CHARACTER:
		break;
	case SOCIAL:
		break;
	case SHOP:
		break;
	case BRAWLER:
		break;
	default:
		break;
	}
	mBtnMsg = NULL;

	return true;
}

void UIManager::PlayButtonClicked()
{
}

bool UIManager::PrintConsoleClicked()
{
	printf("%d button pushed\n", mBtnMsg);
	if (mBtnMsg)
		return true;
	else
		return false;
}

void UIManager::DrawBackground(HDC hdc)
{
	HBITMAP hBitmap, hOldBitmap;
	BITMAP bit;
	HDC HMemDC = CreateCompatibleDC(hdc);
	int bx, by;
	
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT("image/lobby_background.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hOldBitmap = (HBITMAP)SelectObject(HMemDC, hBitmap);
	GetObject(hBitmap, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, 0, 0, bx, by, HMemDC, 0, 0, SRCCOPY);
	//StretchBlt(hdc, 0, 0, WIN_SIZEX, WIN_SIZEY, HMemDC, 0, 0, bx, by, SRCCOPY);
	
	DeleteObject(hBitmap);
	DeleteDC(HMemDC);

}
