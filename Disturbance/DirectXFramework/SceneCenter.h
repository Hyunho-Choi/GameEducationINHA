#pragma once
#include <thread>

#include "SceneFactory.h"

enum class eSceneName
{
	START_SCENE = 0, INGAME_SCENE, LOADING_SCENE
};

class Scene;

class SceneCenter : public BaseObserver
{
private :
	Scene* mLoadingScene;
	thread mLoadingThread;
	void LoadScene(eSceneName sceneName);
	void EnterScene(eSceneName sceneName);
public :
	SceneCenter();
	~SceneCenter() override;
	void ChangeScene(eSceneName sceneName);
	void RegisterScene(Scene* scene);
	void InputProcess();
	void Update();
	bool Update(eEventName eventName, void* parameter) override;
	void Render();

	Scene* GetScene(eSceneName sceneName)
	{
		return mSceneMap[sceneName];
	}

	Scene* GetCurScene()
	{
		return mCurScene;
	}

	
private:
	SceneFactory mSceneFactory;
	Scene* mCurScene;
	std::map<eSceneName, Scene*> mSceneMap;
};
