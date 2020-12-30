#include "stdafx.h"
#include "TimeManager.h"


void TimeManager::Update()
{
	mNewTime = GetCurTime();
	mDeltaTime = mNewTime - mLastTime;
	mLastTime = mNewTime;

	timeStack += GetDeltaTime();
	timeDuringStart += GetDeltaTime();
	fpsCnt++;

	if (timeStack > 1.f)
	{
		cout << "fps : " << fpsCnt << endl;
		fpsCnt = 0;
		timeStack = 0.f;
	}
}

float TimeManager::GetDeltaTime()
{
	// TODO 최초부분 코드 추가 필요

	return mDeltaTime.count();
}

float TimeManager::GetDuringStartTime()
{
	return timeDuringStart;
}

system_clock::time_point TimeManager::GetCurTime()
{
	return system_clock::now();
}
