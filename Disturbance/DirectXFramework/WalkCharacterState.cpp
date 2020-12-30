#include "stdafx.h"
#include "WalkCharacterState.h"
#include "Player.h"
#include "SkinnedMesh.h"
#include "KeyboardInputManager.h"
#include "IdleCharacterState.h"
#include "InteractCharacterState.h"

WalkCharacterState::WalkCharacterState()
	: mPrevMoveVelocity(0,0,0)
	, mMoveVelocity(0,0,0)
{
	mStateName = eCharacterStateName::WALK_STATE;
}

WalkCharacterState::~WalkCharacterState()
{
}

void WalkCharacterState::Enter(Player& player)
{
	player.GetSkinnedMesh()->SetAnimationIndexBlend(4);
	mPrevMoveVelocity = D3DXVECTOR3(0, 0, 0);
	mMoveVelocity = D3DXVECTOR3(0, 0, 0);
}

CharacterState* WalkCharacterState::HandleInput(Player& player, eEventName eventName, eKeyName &key)
{
	if(eventName == eEventName::KEY_DOWN)
	{
		switch (key)
		{
			case eKeyName::KEY_INTERACTION :
				{
					if (player.GetInteractingObject() != nullptr)
					{
						player.SetMoveVelocity(D3DXVECTOR3(0, 0, 0));
						return new InteractCharacterState;
					}
				}
				break;
			case eKeyName::KEY_FRONT:
				{
					mMoveVelocity.z += 1;
					mMoveVelocity.x += 1;
				}
				break;
			case eKeyName::KEY_BACK:
				{
					mMoveVelocity.z -= 1;
					mMoveVelocity.x -= 1;
				}
				break;
			case eKeyName::KEY_LEFT:
				{
					mMoveVelocity.z += 1;
					mMoveVelocity.x -= 1;
				}
				break;
			case eKeyName::KEY_RIGHT:
				{
					mMoveVelocity.z -= 1;
					mMoveVelocity.x += 1;
				}
				break;
		}

		if (!gSoundManager->Playing("WALK"))
		{
			gSoundManager->Play("WALK");
		}


	}
	else if (eventName == eEventName::KEY_UP)
	{
		gSoundManager->Stop("WALK");
	}
	/*else if(eventName == eEventName::KEY_UP)
	{
		switch (key)
		{
			case eKeyName::KEY_FRONT:
			case eKeyName::KEY_BACK :
			case eKeyName::KEY_LEFT :
			case eKeyName::KEY_RIGHT:
				return new IdleCharacterState;
				break;
		}
	}*/
	return nullptr;
}

CharacterState* WalkCharacterState::Update(Player& player)
{
	if (D3DXVec3Length(&mMoveVelocity) == 0.f)
	{
		player.SetMoveVelocity(mMoveVelocity);
		return new IdleCharacterState;
	}
	if(mMoveVelocity != mPrevMoveVelocity)
	{
		/*D3DXQUATERNION & playerRot = player.GetRotRef();
		
		D3DXMATRIXA16 matLook;
		D3DXMatrixLookAtLH(&matLook, &D3DXVECTOR3(0, 0, 0), &mMoveVelocity, &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixInverse(&matLook, nullptr, &matLook);

		D3DXQUATERNION moveRot;
		D3DXQuaternionRotationMatrix(&moveRot, &matLook);

		D3DXQuaternionRotationYawPitchRoll(&playerRot, D3DX_PI, 0, 0);
		playerRot *= moveRot;*/ //이동 방향으로 회전하는 코드
		mPrevMoveVelocity = mMoveVelocity;
	}
	D3DXVec3Normalize(&mMoveVelocity, &mMoveVelocity);
	player.SetMoveVelocity(mMoveVelocity * gTimeManager->GetDeltaTime() * walkSpeed);
	mMoveVelocity = D3DXVECTOR3(0,0,0);
	return nullptr;
}

