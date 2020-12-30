#pragma once

#include "Chaser.h"
#include "SkinnedMesh.h"

enum class eChaserStateName
{
	IDLE = 0, RETURNING, MOVING, ATTACKING
};

class ChaserState
{
private :
protected:
	DWORD mLastUpdateTime;
	eChaserStateName mChaserStateName;
	Chaser* mChaser;
public:
	ChaserState();
	
	virtual void Enter(Chaser* chaser) { mChaser = chaser; }
	virtual ChaserState* Update(Chaser* chaser) { return nullptr; }

	eChaserStateName GetChaserStateName() const { return mChaserStateName; }
};

