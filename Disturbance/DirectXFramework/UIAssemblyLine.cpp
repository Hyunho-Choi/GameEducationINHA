#include "stdafx.h"
#include "UIAssemblyLine.h"
#include "Scene.h"
#include "UIAnimation.h"
#include "UIConversation.h"

void UIAssemblyLine::MakeStartSceneUI(Scene* newScene)
{
	UIImage* uiImage= new UIImage("Resources/UI/Setting/LayerCom.jpg", { 0, 0 }, 64, 36);
	uiImage->SetObjectName("Layer");
	uiImage->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("Layer", uiImage));
	
	uiImage = new UIImage("Resources/UI/Main/Title.png", { 20, 4 }, 24, 8);
	uiImage->SetObjectName("Title");
	uiImage->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("Title", uiImage));

	uiImage = new UIImage("Resources/UI/Main/NewGame.png", { 26,20 }, 12, 4);
	uiImage->SetObjectName("BtnStart");
	uiImage->EventProcess = BtnStartEventListen;
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DOWN, 9, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_UP, 9, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_MOVE, 9, uiImage);
	uiImage->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("btnStart", uiImage));

	uiImage = new UIImage("Resources/UI/Main/Quit.png", { 26, 24 }, 12, 4);
	uiImage->SetObjectName("BtnExit");
	uiImage->EventProcess = BtnExitEventListen;
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DOWN, 9, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_UP, 9, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_MOVE, 9, uiImage);
	uiImage->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("btnExit", uiImage));

	
}

void UIAssemblyLine::MakeIngameSceneUI(Scene* newScene)
{
	UIImage* uiImage = new UIImage("Resources/UI/Setting/Layer2.png", { 17, 6 }, 30, 25);
	uiImage->SetObjectName("SettingLayer");
	uiImage->EventProcess = BtnSettingEventListen;
	newScene->AddEventSubscriberList(eEventName::KEY_UP, 0, uiImage);
	newScene->AddEventSubscriberList(eEventName::KEY_DOWN, 0, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DOWN, 0, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_UP, 0, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_MOVE, 0, uiImage);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DRAG, 0, uiImage);
	uiImage->SetVisible(false);
	newScene->mGameUIs.insert(make_pair("SettingLayer", uiImage));

	UIImage* childUI = new UIImage("Resources/UI/Setting/Exit2_off.png", { 25, 21 }, 13, 2);
	childUI->SetObjectName("ExitBtn");
	childUI->SetUIPath("Resources/UI/Setting/Exit2_off.png", eStateUI::off);
	childUI->SetUIPath("Resources/UI/Setting/Exit2_on.png", eStateUI::on);
	childUI->SetVisible(true);
	uiImage->AddChild("ExitBtn", childUI);

	childUI = new UIImage("Resources/UI/Setting/BgmBox.png", { 25, 12 }, 3, 3);
	childUI->SetObjectName("BGMText");
	childUI->SetUIPath("Resources/UI/Setting/BGM_off.png", eStateUI::off);
	childUI->SetUIPath("Resources/UI/Setting/BGM_on.png", eStateUI::on);
	childUI->SetVisible(true);
	uiImage->AddChild("BGMText", childUI);

	childUI = new UIImage("Resources/UI/Setting/BgmLine.png", { 28, 11 }, 10, 4);
	childUI->SetObjectName("BGMLine");
	childUI->SetVisible(true);
	uiImage->AddChild("BGMLine", childUI);

	childUI = new UIImage("Resources/UI/Setting/BgmButton.png", { 32, 13 }, 1, 1);
	childUI->SetObjectName("BGMBtn");
	childUI->SetUIPath("Resources/UI/Setting/BgmButton.png", eStateUI::off);
	childUI->SetUIPath("Resources/UI/Setting/BgmButton.png", eStateUI::on);
	childUI->SetVisible(true);
	uiImage->GetChildUI("BGMLine")->AddChild("BGMBtn", childUI);

	childUI = new UIImage("Resources/UI/Setting/key_off.png", { 25, 15 }, 13, 3);
	childUI->SetObjectName("Control");
	childUI->SetUIPath("Resources/UI/Setting/key_off.png", eStateUI::off);
	childUI->SetUIPath("Resources/UI/Setting/key_on.png", eStateUI::on);
	childUI->SetVisible(true);
	uiImage->AddChild("Control", childUI);

	childUI = new UIImage("Resources/UI/Setting/etc.png", { 25, 18 }, 13, 3);
	childUI->SetObjectName("Etc");
	childUI->SetVisible(true);
	uiImage->AddChild("Etc", childUI);

	childUI = new UIImage("Resources/UI/Setting/Control.png", { 12, 6 }, 40, 24);
	childUI->SetObjectName("ControlLayer");
	childUI->SetVisible(false);
	uiImage->AddChild("ControlLayer", childUI);

	childUI = new UIImage("Resources/UI/Setting/Return_off.png", { 14, 28 }, 4, 1);
	childUI->SetObjectName("ReturnBtn");
	childUI->SetUIPath("Resources/UI/Setting/Return_off.png", eStateUI::off);
	childUI->SetUIPath("Resources/UI/Setting/Return_on.png", eStateUI::on);
	childUI->SetVisible(false);
	uiImage->AddChild("ReturnBtn", childUI);
	/*
	UIImage* sideFilter = new UIImage("Resources/Shader/round.png", { 0, 0 }, 64, 36);
	sideFilter->SetObjectName("sideFilter");
	sideFilter->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("sideFilter", sideFilter));
	*/
	UIImage* uiLifeBar = new UIImage("Resources/UI/LifeBar.png", { 23, 0 }, 20, 5);
	uiLifeBar->SetObjectName("LifeBar");
	uiLifeBar->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("LifeBar", uiLifeBar));

	UIAnimation* animUi = new UIAnimation("Resources/UI/Life.png", { 24, 1 }, 18, 4, 20);
	animUi->SetObjectName("LifeLine");
	animUi->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("LifeLine", animUi));

	UIConversation* uitest = new UIConversation();
	uitest->EventProcess = BtnConversationUI;
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DOWN, 0, uitest);
	newScene->AddEventSubscriberList(eEventName::KEY_DOWN, 0, uitest);
	uitest->SetObjectName("conversation");
	uitest->SetVisible(false);
	newScene->mGameUIs.insert(make_pair("conversation", uitest));


	UIImage* MiniGameUI = new UIImage("Resources/UI/MiniGame/GameScene.png", { 5,2 }, 54, 32);
	MiniGameUI->SetObjectName("MiniGameLayer");
	MiniGameUI->EventProcess = MiniGameEventListen;
	newScene->AddEventSubscriberList(eEventName::KEY_UP, 0, MiniGameUI);
	newScene->AddEventSubscriberList(eEventName::KEY_DOWN, 0, MiniGameUI);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DOWN, 0, MiniGameUI);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_UP, 0, MiniGameUI);
	newScene->AddEventSubscriberList(eEventName::MOUSE_MOVE, 0, MiniGameUI);
	newScene->AddEventSubscriberList(eEventName::MOUSE_L_DRAG, 0, MiniGameUI);
	MiniGameUI->SetVisible(false);
	newScene->mGameUIs.insert(make_pair("MniGameLayer", MiniGameUI));

	UIImage* MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/claw_line.png", { 44, 2 }, 5, 6);
	MiniGameObjectUI->SetObjectName("ClawLine");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("ClawLine", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/claw_hold.png", { 44,2 }, 5, 12);
	MiniGameObjectUI->SetObjectName("Claw");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->GetChildUI("ClawLine")->AddChild("Claw", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/briefcase.png", { 6, 17 }, 12, 6);
	MiniGameObjectUI->SetObjectName("briefcase");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("briefcase", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/tumbler.png", { 8, 20 }, 3, 4);
	MiniGameObjectUI->SetObjectName("tumbler");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("tumbler", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/notebook.png", { 14, 18 }, 8, 5);
	MiniGameObjectUI->SetObjectName("notebook");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("notebook", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/pen_holder.png", { 21, 18 }, 4, 5);
	MiniGameObjectUI->SetObjectName("pen_holder");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("pen_holder", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/watch.png", { 25, 21 }, 3, 2);
	MiniGameObjectUI->SetObjectName("watch");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("watch", MiniGameObjectUI);

	MiniGameObjectUI = new UIImage("Resources/UI/MiniGame/telephone.png", { 27, 19 }, 10, 6);
	MiniGameObjectUI->SetObjectName("telephone");
	MiniGameObjectUI->SetVisible(true);
	MiniGameUI->AddChild("telephone", MiniGameObjectUI);
}

void UIAssemblyLine::MakeLoadingSceneUI(Scene* newScene)
{
	UIImage* loadingMain = new UIImage("Resources/UI/Loading/Loading_UI_Main.png", { 0, 0 }, 64, 36);
	loadingMain->SetObjectName("loadingMain");
	loadingMain->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("loadingMain", loadingMain));

	UIAnimation* loadingSpin = new UIAnimation("Resources/UI/Loading/Loading_Spin.png", { 56, 30 }, 4, 4, 4);
	loadingSpin->SetObjectName("loadingSpin");
	loadingSpin->SetVisible(true);
	newScene->mGameUIs.insert(make_pair("loadingSpin", loadingSpin));
}

void UIAssemblyLine::MakeSceneUI(Scene* newScene)
{
	D3DVIEWPORT9 viewPort;
	gD3Device->GetViewport(&viewPort);
	switch (newScene->GetSceneName())
	{
		case eSceneName::START_SCENE :
			{
				MakeStartSceneUI(newScene);
			}
			break;
		case eSceneName::INGAME_SCENE :
			{
				MakeIngameSceneUI(newScene);
			}
			break;
		case eSceneName::LOADING_SCENE:
			{
				MakeLoadingSceneUI(newScene);
			}
			break;
		default :
			{
				
			}
			break;
	}
}
