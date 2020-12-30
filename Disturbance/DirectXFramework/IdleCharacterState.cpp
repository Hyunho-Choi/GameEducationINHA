#include "stdafx.h"
#include "IdleCharacterState.h"
#include "KeyboardInputManager.h"
#include "WalkCharacterState.h"
#include "Player.h"
#include "SkinnedMesh.h"
#include "InteractCharacterState.h"

IdleCharacterState::IdleCharacterState()
	: mMoveVelocity(0,0,0)
	, idleStartTime(0)
	, mbAnimationChanged(false)
{
	mStateName = eCharacterStateName::IDLE_STATE;
}

IdleCharacterState::~IdleCharacterState()
{
}

void IdleCharacterState::Enter(Player& player)
{
	idleStartTime = GetTickCount();
	mbAnimationChanged = false;
	mMoveVelocity = D3DXVECTOR3(0, 0, 0);
	player.SetMoveVelocity(mMoveVelocity);
}

CharacterState* IdleCharacterState::HandleInput(Player& player, eEventName eventName, eKeyName &key)
{
	if(key == eKeyName::KEY_INTERACTION)
	{
		if (player.GetInteractingObject() != nullptr)
		{
			player.SetMoveVelocity(D3DXVECTOR3(0, 0, 0));
			return new InteractCharacterState;
		}
	}
	else if (eventName == eEventName::KEY_DOWN)
	{
		switch (key)
		{
			case eKeyName::KEY_FRONT:
				{
					mMoveVelocity.z += 1;
				}
				break;
			case eKeyName::KEY_BACK:
				{
					mMoveVelocity.z -= 1;
				}
				break;
			case eKeyName::KEY_LEFT:
				{
					mMoveVelocity.x -= 1;
				}
				break;
			case eKeyName::KEY_RIGHT:
				{
					mMoveVelocity.x += 1;
				}
				break;
		}
	}
	return nullptr;
}

CharacterState* IdleCharacterState::Update(Player& player)
{
	if (D3DXVec3Length(&mMoveVelocity) != 0.f)
	{
		return new WalkCharacterState;
	}

	if(mbAnimationChanged == false && GetTickCount() - idleStartTime > 50)
	{
		mbAnimationChanged = true;
		player.GetSkinnedMesh()->SetAnimationIndexBlend(5);
	}

	return nullptr;
}
