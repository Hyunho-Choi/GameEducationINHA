#pragma once

#include "EventManager.h"

class BaseObserver
{
public:
	BaseObserver();
	virtual ~BaseObserver();

	virtual bool Update(eEventName eventName, void* parameter) = 0;
};
