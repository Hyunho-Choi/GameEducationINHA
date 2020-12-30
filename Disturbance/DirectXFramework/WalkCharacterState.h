#pragma once
#include "CharacterState.h"
class WalkCharacterState :
	public CharacterState
{
	D3DXVECTOR3 mPrevMoveVelocity;
	D3DXVECTOR3 mMoveVelocity;

	static const UINT walkSpeed = 7;
public:
	WalkCharacterState();
	~WalkCharacterState();
	
	void Enter(Player& player) override;
	CharacterState* HandleInput(Player& player, eEventName eventName, eKeyName &key) override;
	CharacterState* Update(Player& player) override;
};

