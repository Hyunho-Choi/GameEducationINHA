#pragma once
#include "Camera.h"
#include "SceneCenter.h"

class BaseObject;
class QuarterMap;

class Scene
{
	friend SceneFactory;
	eSceneName mSceneName;
	Camera * mCamera;
	map<eEventName, multimap<UINT, BaseObserver*>> mEventSubscriberList;
public:
	Scene(eSceneName sceneName);
	virtual ~Scene();

	map<string, BaseObject*> mGameObjects;
	map<string, BaseObject*> mGameUIs;

	eSceneName GetSceneName() const
	{
		return mSceneName;
	}

	void AddEventSubscriberList(eEventName eventName, UINT priority, BaseObserver* baseObserver);

	void AttachAllSubscriberInSubscriberList();
	void DetachAllSubscriberInSubscriberList();

	
	virtual void Update();
	virtual void Render();

	Camera* GetCamera();

private:
};
