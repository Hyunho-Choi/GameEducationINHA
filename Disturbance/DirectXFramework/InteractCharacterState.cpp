#include "stdafx.h"
#include "InteractCharacterState.h"
#include "Player.h"
#include "SkinnedMesh.h"
#include "IdleCharacterState.h"
#include "Interactable3DObject.h"

InteractCharacterState::InteractCharacterState()
	: interactStartTime(0)
	, AnimationPeriod(0)
{
	mStateName = eCharacterStateName::INTERACTION_STATE;
}

InteractCharacterState::~InteractCharacterState()
{
}

void InteractCharacterState::Enter(Player& player)
{
	interactStartTime = GetTickCount();
	if(player.GetInteractingObject()->GetPos().y > player.GetPos().y)
	{
		player.GetSkinnedMesh()->SetAnimationIndexBlend(1);
	}
	else
	{
		player.GetSkinnedMesh()->SetAnimationIndexBlend(2);
	}
	LPD3DXANIMATIONSET curAnimSet;
	player.GetSkinnedMesh()->m_pAnimController->GetTrackAnimationSet(0, &curAnimSet);
	AnimationPeriod = curAnimSet->GetPeriod();

	D3DXQUATERNION quatRot;
	D3DXMATRIXA16 matRot;
	D3DXVECTOR3 interactingObjectPos = player.GetInteractingObject()->GetPos();
	interactingObjectPos.y = player.GetPos().y;

	D3DXMatrixLookAtLH(&matRot, &(player.GetPos()), &interactingObjectPos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&matRot, nullptr, &matRot);
	D3DXQuaternionRotationMatrix(&quatRot, &matRot);

	D3DXQUATERNION playerIdleRot;
	D3DXQuaternionRotationYawPitchRoll(&playerIdleRot, D3DX_PI, 0, 0);
	player.SetRot(playerIdleRot * quatRot);
}

CharacterState* InteractCharacterState::Update(Player& player)
{
	if(GetTickCount() - interactStartTime > (AnimationPeriod -  player.GetSkinnedMesh()->GetBlendTime() - 0.1f) * 1000 )
	{
		//player.GetInteractingObject()->DeleteInRoom(player.GetInteractingObject()->GetObjectName());
		Interactable3DObject* interactable3DObject = (Interactable3DObject *)player.GetInteractingObject();
		interactable3DObject->TryInteraction();
		player.SetInteractingObject(nullptr);
		return new IdleCharacterState;
	}
	return nullptr;
}
