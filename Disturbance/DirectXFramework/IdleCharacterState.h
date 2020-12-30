#pragma once
#include "CharacterState.h"
class IdleCharacterState :
	public CharacterState
{
	D3DXVECTOR3 mMoveVelocity;
	DWORD idleStartTime;
	bool mbAnimationChanged;
public:
	IdleCharacterState();
	~IdleCharacterState();

	void Enter(Player& player) override;
	CharacterState* HandleInput(Player& player, eEventName eventName, eKeyName &key) override;
	CharacterState* Update(Player& player) override;
};

