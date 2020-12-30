#pragma once

#include "Singleton.h"

#define gTimeManager TimeManager::GetInstance()
#define gDeltaTime TimeManager::GetInstance()->GetDeltaTime()

using namespace chrono;

class TimeManager : public Singleton<TimeManager>
{
private:
	int fpsCnt = 0;
	float timeStack = 0.f;
	float timeDuringStart = 0.f;
	system_clock::time_point mLastTime;
	system_clock::time_point mNewTime;
	duration<float> mDeltaTime;
	system_clock::time_point GetCurTime();

public:
	void Update();
	float GetDeltaTime();
	float GetDuringStartTime();
};
