#pragma once

#include "Singleton.h"

#define gEventManager EventManager::GetInstance()

enum class eEventName
{
	SCENE_CHANGE = 0, PLAYER_DETECTED,
	MOUSE_L_DOWN, MOUSE_R_DOWN, MOUSE_L_UP, MOUSE_R_UP, MOUSE_L_DRAG, MOUSE_R_DRAG, MOUSE_WHEEL_DOWN, MOUSE_WHEEL_SCROLL, MOUSE_MOVE, KEY_DOWN, KEY_UP
};

class BaseObserver;

class EventManager : public Singleton<EventManager>
{
	map<eEventName, multimap<UINT, BaseObserver*>> mMapSubscriber;
public:

	void ClearSubscriber();
	
	bool AttachSubscriber(eEventName eventName, UINT priority, BaseObserver* newSubscriber)
	{
		mMapSubscriber[eventName].insert(make_pair(priority, newSubscriber));
		return true;
	}

	void DetachSubscriber(eEventName eventName, BaseObserver* detachedSubscriber)
	{
		for (auto it = mMapSubscriber[eventName].begin(); it != mMapSubscriber[eventName].end(); it++)
		{
			if (it->second == detachedSubscriber)
			{
				mMapSubscriber[eventName].erase(it);
				break;
			}
		}
	}

	void DetachSubscriber(BaseObserver* detachedSubscriber)
	{
		for (auto it_map = mMapSubscriber.begin(); it_map != mMapSubscriber.end(); it_map++)
		{
			for (auto it_mapVec = it_map->second.begin(); it_mapVec != it_map->second.end(); it_mapVec++)
			{
				if (it_mapVec->second == detachedSubscriber)
				{
					it_map->second.erase(it_mapVec);
					break;
				}
			}
		}
	}

	void EventOccurred(eEventName eventName, void* parameter);
};

