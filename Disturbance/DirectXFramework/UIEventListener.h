#pragma once

#include "DeviceManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "UIBase.h"
#include "UIImage.h"
#include "SceneCenter.h"
#include "KeyboardInputManager.h"

inline bool BtnStartEventListen(eEventName eventName, void * parameter, UIBase * uiImage)
{
	UIImage* uiImageConvert = (UIImage*)uiImage;
	static bool isClicked = false;
	switch (eventName)
	{
	case eEventName::MOUSE_L_DOWN:
	{
		POINT& mousePt = *(POINT*)(parameter);
		if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
		{
			uiImageConvert->SetTexture("Resources/UI/Main/NewGame_Click.png");
			isClicked = true;
			return true;
		}
	}
	break;
	case eEventName::MOUSE_L_UP:
	{
		POINT& mousePt = *(POINT*)(parameter);
		isClicked = false;
		if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
		{
			eSceneName sceneName = eSceneName::INGAME_SCENE;
			gSoundManager->Play("CLICK");
			gEventManager->EventOccurred(eEventName::SCENE_CHANGE, &(sceneName));
			uiImageConvert->SetTexture("Resources/UI/Main/NewGame.png");
			return true;
		}
		uiImageConvert->SetTexture("Resources/UI/Main/NewGame.png");
	}
	break;
	case eEventName::MOUSE_MOVE:
	{
		POINT& mousePt = *(POINT*)(parameter);
		if (isClicked == false)
		{
			if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->SetTexture("Resources/UI/Main/NewGame_Click.png");
			}
			else
			{
				uiImageConvert->SetTexture("Resources/UI/Main/NewGame.png");
			}
		}
	}
	break;
	default:
		break;
	}
	return false;
}

inline bool BtnExitEventListen(eEventName eventName, void* parameter, UIBase* uiImage)
{
	UIImage* uiImageConvert = (UIImage*)uiImage;
	static bool isClicked = false;
	switch (eventName)
	{
	case eEventName::MOUSE_L_DOWN:
	{
		POINT mousePt = *(POINT*)(parameter);
		if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
		{
			uiImageConvert->SetTexture("Resources/UI/Main/Quit_Click.png");
			isClicked = true;
			return true;
		}
	}
	break;
	case eEventName::MOUSE_L_UP:
	{
		POINT mousePt = *(POINT*)(parameter);
		isClicked = false;
		if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
		{
			gSoundManager->Play("CLICK");
			uiImageConvert->SetTexture("Resources/UI/Main/Quit.png");
			return true;
		}
		uiImageConvert->SetTexture("Resources/UI/Main/Quit.png");
	}
	break;
	case eEventName::MOUSE_MOVE:
	{
		POINT mousePt = *(POINT*)(parameter);
		if (isClicked == false)
		{
			if (PtInRect(&uiImageConvert->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->SetTexture("Resources/UI/Main/Quit_Click.png");
			}
			else
			{
				uiImageConvert->SetTexture("Resources/UI/Main/Quit.png");
			}
		}
	}
	break;
	default:
		break;
	}


	return false;
}

inline bool BtnSettingEventListen(eEventName eventName, void* parameter, UIBase* uiImage)
{
	UIImage* uiImageConvert = (UIImage*)uiImage;
	static bool isClicked = false;
	static int isCalled = -1;

	switch (eventName)
	{
	case eEventName::KEY_UP:
	{
		eKeyName key = *(eKeyName*)parameter;
		if (key == eKeyName::KEY_ESC)
		{
			isCalled *= -1;
		}

		if (isCalled == 1)
		{
			static int temp = 0;
			switch (key)
			{
			case eKeyName::KEY_FRONT:
				temp -= 1;
				break;

			case eKeyName::KEY_BACK:
				temp += 1;
				break;

			case eKeyName::KEY_INTERACTION:

				break;
			default:
				break;
			}
		
			
		}
	}
	break;
	case eEventName::KEY_DOWN:
	{
		/*eKeyName key = *(eKeyName*)parameter;

		switch (key)
		{
		case eKeyName::KEY_FRONT:

			break;

		case eKeyName::KEY_BACK:

			break;

		case eKeyName::KEY_INTERACTION:

			break;
		}		*/
	}
	break;
	case eEventName::MOUSE_L_DOWN:
	{
		POINT mousePt = *(POINT*)(parameter);
		if (PtInRect(&uiImageConvert->GetChildUI("ExitBtn")->GetRectInViewPort(), mousePt) && uiImageConvert->GetChildUI("ExitBtn")->IsVisible())
		{
			uiImageConvert->GetChildUI("ExitBtn")->SetTexture(eStateUI::on);
			isClicked = true;
		}

		if (PtInRect(&uiImageConvert->GetChildUI("Control")->GetRectInViewPort(), mousePt))
		{
			uiImageConvert->GetChildUI("Control")->SetTexture(eStateUI::on);
			isClicked = true;
		}

		if (PtInRect(&uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn")->GetRectInViewPort(), mousePt))
		{
			uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn")->SetTexture(eStateUI::on);
			isClicked = true;
		}


		if (PtInRect(&uiImageConvert->GetChildUI("ReturnBtn")->GetRectInViewPort(), mousePt) && uiImageConvert->GetChildUI("ReturnBtn")->IsVisible())
		{
			uiImageConvert->GetChildUI("ReturnBtn")->SetTexture(eStateUI::on);
			isClicked = true;
		}

	}
	break;
	case eEventName::MOUSE_L_UP:
	{
		POINT mousePt = *(POINT*)(parameter);
		isClicked = false;
		if (PtInRect(&uiImageConvert->GetChildUI("ExitBtn")->GetRectInViewPort(), mousePt) && uiImageConvert->GetChildUI("ExitBtn")->IsVisible())
		{
			uiImageConvert->GetChildUI("ExitBtn")->SetTexture(eStateUI::off);
			isCalled = -1;
		}

		if (PtInRect(&uiImageConvert->GetChildUI("Control")->GetRectInViewPort(), mousePt))
		{

			uiImageConvert->GetChildUI("Control")->SetTexture(eStateUI::off);

			for (pair<string, UIImage*> child : uiImageConvert->GetChildUI())
			{
				if (child.first == "ControlLayer" || child.first == "ReturnBtn")
				{
					child.second->SetVisible(true);
				}
				else
				{
					child.second->SetVisible(false);
				}
			}

		}

		if (PtInRect(&uiImageConvert->GetChildUI("BGMLine")->GetRectInViewPort(), mousePt))
		{
			UIImage*Line = uiImageConvert->GetChildUI("BGMLine");
			UIImage*Btn = uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn");

			int num = (mousePt.x - Line->GetPos().x) / (Line->GetWidth()*0.1f);
			LONG x = Line->GetPos().x + ((Line->GetWidth()*0.1)* num);
			POINT pos = { x, Btn->GetPos().y };
			Btn->SetPos(pos);
			gSoundManager->Volume("PartA_BGM", num*0.1f);
		}


		if (PtInRect(&uiImageConvert->GetChildUI("ReturnBtn")->GetRectInViewPort(), mousePt) && uiImageConvert->GetChildUI("ReturnBtn")->IsVisible())
		{
			uiImageConvert->GetChildUI("ReturnBtn")->SetTexture(eStateUI::off);
			for (pair<string, UIImage*> child : uiImageConvert->GetChildUI())
			{
				if (child.first == "ControlLayer" || child.first == "ReturnBtn")
				{
					child.second->SetVisible(false);
				}
				else
				{
					child.second->SetVisible(true);
				}
			}
		}

	}
	break;
	case eEventName::MOUSE_MOVE:
	{
		POINT mousePt = *(POINT*)(parameter);
		if (isClicked == false)
		{
			if (PtInRect(&uiImageConvert->GetChildUI("ExitBtn")->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->GetChildUI("ExitBtn")->SetTexture(eStateUI::on);
			}
			else
			{
				uiImageConvert->GetChildUI("ExitBtn")->SetTexture(eStateUI::off);
			}

			if (PtInRect(&uiImageConvert->GetChildUI("Control")->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->GetChildUI("Control")->SetTexture(eStateUI::on);
			}
			else
			{
				uiImageConvert->GetChildUI("Control")->SetTexture(eStateUI::off);
			}

			if (PtInRect(&uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn")->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn")->SetTexture(eStateUI::on);
			}
			else
			{
				uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn")->SetTexture(eStateUI::off);
			}

			if (PtInRect(&uiImageConvert->GetChildUI("ReturnBtn")->GetRectInViewPort(), mousePt))
			{
				uiImageConvert->GetChildUI("ReturnBtn")->SetTexture(eStateUI::on);
			}
			else
			{
				uiImageConvert->GetChildUI("ReturnBtn")->SetTexture(eStateUI::off);
			}


		}
		else
		{
			if (PtInRect(&uiImageConvert->GetChildUI("BGMLine")->GetRectInViewPort(), mousePt))
			{
				UIImage*Line = uiImageConvert->GetChildUI("BGMLine");
				UIImage*Btn = uiImageConvert->GetChildUI("BGMLine")->GetChildUI("BGMBtn");

				int num = (mousePt.x - Line->GetPos().x) / (Line->GetWidth()*0.1f);
				LONG x = Line->GetPos().x + ((Line->GetWidth()*0.1)* num);
				POINT pos = { x, Btn->GetPos().y };
				Btn->SetPos(pos);
				gSoundManager->Volume("PartA_BGM", num*0.1f);
			}
		}
	}
	break;
	default:
		break;
	}


	if (isCalled == 1)
	{
		uiImageConvert->SetVisible(true);
		return true;
	}
	else
	{
		for (pair<string, UIImage*> child : uiImageConvert->GetChildUI())
		{
			if (child.first == "ControlLayer" || child.first == "ReturnBtn")
			{
				child.second->SetVisible(false);
			}
			else
			{
				child.second->SetVisible(true);
			}
		}
		uiImageConvert->SetVisible(false);
		return false;
	}

}


inline bool MiniGameEventListen(eEventName eventName, void* parameter, UIBase* uiImage)
{
	UIImage* uiImageConvert = (UIImage*)uiImage;

	switch (eventName)
	{
	case eEventName::KEY_UP:
	{
		eKeyName key = *(eKeyName*)parameter;
		POINT temp = uiImageConvert->GetChildUI("ClawLine")->GetChildUI("Claw")->GetPos();
		POINT temp2 = { 2 ,1 };
		if (key == eKeyName::KEY_INTERACTION)
		{
			while(temp.y >1)
			{
				temp.y -= temp2.y;
				uiImageConvert->GetChildUI("ClawLine")->GetChildUI("Claw")->SetPos(temp);
			}
		}

	}
	break;


	case eEventName::KEY_DOWN:
	{
		eKeyName key = *(eKeyName*)parameter;
		POINT temp = uiImageConvert->GetChildUI("ClawLine")->GetPos();
		uiImageConvert->GetChildUI("ClawLine")->GetChildUI("Claw")->SetPos(temp);
		POINT temp2 = { 2 ,3 };
		if (key == eKeyName::KEY_LEFT)
		{
			temp.x -= temp2.x;
		}
		else if (key == eKeyName::KEY_RIGHT)
		{
			temp.x += temp2.x;
		}
		else if (key == eKeyName::KEY_INTERACTION)
		{
			temp.y += temp2.y;
		}
		uiImageConvert->GetChildUI("ClawLine")->SetPos(temp);

	}
	break;


	default:
		break;
	}



	return false;
}



inline bool BtnConversationUI(eEventName eventName, void* parameter, UIBase* uiImage)
{
	UIImage* uiImageConvert = (UIImage*)uiImage;
	
	switch (eventName)
	{
		case eEventName::KEY_DOWN:
		case eEventName::MOUSE_L_DOWN:
		{
			uiImageConvert->SetVisible(false);
		}
		break;
		default:
		break;
	}
	return false;
}
