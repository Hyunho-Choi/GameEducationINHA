#include "stdafx.h"
#include "EventManager.h"
#include "BaseObserver.h"

void EventManager::ClearSubscriber()
{
	for (pair<const eEventName, multimap<UINT, BaseObserver*>>& subscriberMap : mMapSubscriber)
	{
		subscriberMap.second.clear();
	}
	mMapSubscriber.clear();
}

void EventManager::EventOccurred(eEventName eventName, void* parameter)
{
	for (pair<const UINT, BaseObserver *>& subscriber : mMapSubscriber[eventName])
	{
		if (subscriber.second->Update(eventName, parameter))
		{
			break;
		}
	}
}
