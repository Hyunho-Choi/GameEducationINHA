#include "stdafx.h"

// 디버깅용
#include <iostream>

#include "SceneCenter.h"
#include "Scene.h"
#include "MouseInputManager.h"
#include "KeyboardInputManager.h"
#include "EventManager.h"

using namespace std;

extern CRITICAL_SECTION CS;

void SceneCenter::LoadScene(eSceneName sceneName)
{
	/*int count = 0;
	DWORD last = GetTickCount();
	while(count < 10)
	{
		if(GetTickCount() - last > 30)
		{
			last = GetTickCount();
			cout << "Asd\n";
			count++;
		}
		else
		{
			this_thread::yield();
		}
	}*/
	Scene* newScene = nullptr;
	cout << "load Start" << endl;
	newScene = mSceneFactory.CreateScene(sceneName);
	newScene->AttachAllSubscriberInSubscriberList();
	EnterCriticalSection(&CS);
	EnterScene(newScene->GetSceneName());
	gCameraManager->SetCamera(newScene->GetCamera());
	this->mCurScene = newScene;
	LeaveCriticalSection(&CS);
	cout << "end Loading\n";
}

void SceneCenter::EnterScene(eSceneName sceneName)
{
	switch (sceneName)
	{
		case eSceneName::START_SCENE :
			{
				gSoundManager->Play("Menu_BGM");
			}
			break;
		case eSceneName::INGAME_SCENE :
			{
				gSoundManager->Stop("Menu_BGM");
				gSoundManager->Play("PartA_BGM");
			}
			break;
		default :
			{
				
			}
			break;
	}
}

SceneCenter::SceneCenter()
	: mCurScene(nullptr)
	, mLoadingThread{}
{
	gSoundManager->SoundSet();
	gShader->LoadAllShader();
	
	gEventManager->AttachSubscriber(eEventName::SCENE_CHANGE, 0, this);
	
	mLoadingScene = mSceneFactory.CreateScene(eSceneName::LOADING_SCENE);
	gCameraManager->SetCamera(mLoadingScene->GetCamera());

	ChangeScene(eSceneName::START_SCENE);
}

SceneCenter::~SceneCenter()
{
	if(mLoadingThread.joinable())
	{
		mLoadingThread.join();
	}
	for (auto scene : mSceneMap)
	{
		SAFE_DELETE(scene.second);
	}
	mSceneMap.clear();
}

void SceneCenter::ChangeScene(eSceneName sceneName)
{
	if (mCurScene != nullptr)
	{
		mCurScene->DetachAllSubscriberInSubscriberList();
	}
	SAFE_DELETE(mCurScene);
	mCurScene = mLoadingScene;
	mLoadingThread = std::thread{ &SceneCenter::LoadScene, this, sceneName };
	mLoadingThread.detach();
}

void SceneCenter::RegisterScene(Scene* scene)
{
	if (mSceneMap.find(scene->GetSceneName()) == mSceneMap.end())
	{
		mSceneMap.insert(make_pair(scene->GetSceneName(), scene));
	}
	else
	{
		//TODO Scene이 존재할때 예외처리
	}
}

void SceneCenter::InputProcess()
{
	/*if ( gKeyboardInputManager->ReadKey() != static_cast<int>(eKeyButton::NON_KEY))
	{
	}*/
	gKeyboardInputManager->Update();
}

void SceneCenter::Update()
{
	gTimeManager->Update();

	if (mCurScene != nullptr)
	{
		//cout << static_cast<int>(mCurScene->GetSceneName()) << endl;
		mCurScene->Update();
	}

	gSoundManager->Update();
}

bool SceneCenter::Update(eEventName eventName, void* parameter)
{
	if(eventName == eEventName::SCENE_CHANGE)
	{
		eSceneName * sceneName = static_cast<eSceneName *>(parameter);
		ChangeScene(*sceneName);
	}
	return true;
}

void SceneCenter::Render()
{
	//cout << "Render\n";
	if (mCurScene != nullptr)
	{
		EnterCriticalSection(&CS);
		mCurScene->Render();
		LeaveCriticalSection(&CS);
	}
}
