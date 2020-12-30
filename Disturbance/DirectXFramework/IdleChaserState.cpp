#include "stdafx.h"
#include "IdleChaserState.h"

#include "MovingChaserState.h"

IdleChaserState::IdleChaserState()
{
	mChaserStateName = eChaserStateName::IDLE;
}

IdleChaserState::~IdleChaserState()
{
}

void IdleChaserState::Enter(Chaser* chaser)
{
	ChaserState::Enter(chaser);
	chaser->GetSkinnedMesh()->SetAnimationIndex(8);
}

ChaserState* IdleChaserState::Update(Chaser* chaser)
{
	if(GetTickCount() - mLastUpdateTime > Chaser::findCycleTime)
	{
		mLastUpdateTime = GetTickCount();
		D3DXVECTOR3 playerPos;
		if(chaser->IsPlayerInSight(&playerPos))
		{
			chaser->SetTarget(playerPos);
			return new MovingChaserState;
		}
	}
	return nullptr;
}
