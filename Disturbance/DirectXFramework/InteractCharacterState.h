#pragma once
#include "CharacterState.h"
class InteractCharacterState :
	public CharacterState
{
	DWORD interactStartTime;
	float AnimationPeriod;
public:
	InteractCharacterState();
	~InteractCharacterState();

	void Enter(Player& player) override;
	CharacterState* Update(Player& player) override;
};

