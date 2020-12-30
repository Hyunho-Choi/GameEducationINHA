#include "stdafx.h"
#include "Chaser.h"
#include "SkinnedMesh.h"
#include "RoomCenter.h"
#include "Player.h"
#include "ColliderCube.h"
#include "IdleChaserState.h"
#include "Room.h"

D3DXVECTOR3 Chaser::baseSightDir = { 0, 0, 1 };
float Chaser::baseSightLength = 30.f;
float Chaser::baseSightAngle = D3DX_PI / 5.f;
vector<D3DXPLANE> Chaser::baseSightFrustum;

//void Chaser::FollowingPath()
//{
//	D3DXVECTOR3 moveDir = mPath.back() - mPos;
//	const float distance = D3DXVec3Length(&moveDir);
//	if(distance < 0.1f)
//	{
//		mPos = mPath.back();
//		mPath.pop_back();
//		if (mPath.empty() == false)
//		{
//			RotateToNextNode();
//		}
//		else
//		{
//			if(mChaserState == ChaserState::CHASING)
//			{
//				ReturnToBasePos();
//			}
//			else
//			{
//				mChaserState = ChaserState::IDLE;
//				mSkinnedMesh->SetAnimationIndexBlend(8);
//			}
//		}
//	}
//	else
//	{
//		D3DXVec3Normalize(&moveDir, &moveDir);
//		mPos += moveDir * mSpeed;
//	}
//}

void Chaser::RotateToTarget()
{
	D3DXQUATERNION quatRot;
	D3DXMATRIXA16 matRot;
	D3DXVECTOR3 dir = mTargetPos - mPos;
	D3DXVec3Normalize(&dir, &dir);
	D3DXMatrixLookAtLH(&matRot, &D3DXVECTOR3(0, 0, 0), &dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&matRot, nullptr, &matRot);
	D3DXQuaternionRotationMatrix(&quatRot, &matRot);

	static D3DXQUATERNION idleRot = *D3DXQuaternionRotationYawPitchRoll(&idleRot, D3DX_PI, 0, 0);
	this->SetRot(idleRot * quatRot);
}

bool Chaser::ObjectInSightFrustum(Base3DObject* object, vector<D3DXPLANE>& sightFrustum)
{
	if(D3DXVec3Length(&(mPos - object->GetPos())) > baseSightLength)
	{
		return false;
	}

	for (vector<D3DXPLANE>::value_type plane : sightFrustum)
	{
		if(D3DXPlaneDotCoord(&plane, &object->GetPos()) < 0.f)
		{
			return false;
		}
	}
	
	return true;
}

void Chaser::MakeSightFrustum(vector<D3DXPLANE>& sightPlane)
{
	D3DXMATRIXA16 matR, matT, matWorld;
	D3DXMatrixRotationQuaternion(&matR, &mRot);
	D3DXMatrixTranslation(&matT, mPos.x, mPos.y, mPos.z);
	matWorld = matR * matT;
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXPLANE transformPlane;
	for (vector<D3DXPLANE>::value_type& plane : baseSightFrustum)
	{
		D3DXPlaneNormalize(&transformPlane, &plane);
		D3DXPlaneTransform(&transformPlane, &plane, &matWorld);
		sightPlane.push_back(transformPlane);
	}
}

Chaser::Chaser(D3DXVECTOR3 basePos, RoomCenter* roomCenter)
	: mSpeed(0.05f)
	, mBasePos(basePos)
	, mRoomCenter(roomCenter)
	, mLastUpdateTime(0)
	, mCycleTime(findCycleTime)
{
	mPos = basePos;
	SetScale(D3DXVECTOR3(0.003f, 0.003f, 0.003f));
	AddColliderCube("basicColliderCube");
	mSkinnedMesh = new SkinnedMesh("Resources/XFile/Chaser", "Chaser.X");
	mSkinnedMesh->SetAnimationIndex(8);
	mChaserState = new IdleChaserState;
	mChaserState->Enter(this);
	if(baseSightFrustum.size() == 0)
	{
		GetFrustum((D3DXVECTOR3(0, 0, 0)), Chaser::baseSightDir, Chaser::baseSightLength, Chaser::baseSightAngle, Chaser::baseSightFrustum);
	}
	AddColliderCube("basicColliderCube");
	Base3DObject::Setup();
}

Chaser::~Chaser()
{
	mPath.clear();
	SAFE_DELETE(mSkinnedMesh);
}

void Chaser::MatchTargetToPlayer()
{
	mTargetPos = mRoomCenter->GetPlayer()->GetPos();
}


void Chaser::FollowingPath()
{
}

void Chaser::ReturnToBasePos()
{
}

void Chaser::Attack(Player* player)
{
}

//void Chaser::ReturnToBasePos()
//{
//	mChaserState = ChaserState::RETURNING;
//	mSkinnedMesh->SetAnimationIndexBlend(4);
//	mSpeed = Chaser::basicSpeed;
//	FindPath(mBasePos);
//}
//
//void Chaser::Attack(Player* player)
//{
//	mPath.clear();
//	mChaserState = ChaserState::ATTACKING;
//	mSkinnedMesh->SetAnimationIndexBlend(2);
//	mSkinnedMesh->SetCurrentAnimationSpeed(3);
//	mCycleTime = attackCycleTime;
//}
//
//void Chaser::SetTarget(D3DXVECTOR3& targetPos)
//{
//	if (mChaserState != ChaserState::CHASING)
//	{
//		mChaserState = ChaserState::CHASING;
//		mSkinnedMesh->SetAnimationIndexBlend(6);
//		mSpeed = Chaser::angrySpeed;
//		mCycleTime = findCycleTime;
//	}
//	FindPath(targetPos);
//}

bool Chaser::IsPlayerInSight(OUT D3DXVECTOR3* outPlayerPos)
{
	Player* player = mRoomCenter->GetPlayer();

	vector<D3DXPLANE> sightFrustum;
	MakeSightFrustum(sightFrustum);
	if (ObjectInSightFrustum(player, sightFrustum))
	{
		D3DXVECTOR3 playerPos = player->GetPos();
		D3DXVECTOR3 sightRayDir = playerPos - mPos;
		float distanceToPlayer = D3DXVec3Length(&sightRayDir);
		D3DXVec3Normalize(&sightRayDir, &sightRayDir);
		Room* currentRoom = mRoomCenter->FindRoomIncludePos(mPos);
		if(currentRoom == nullptr)
		{
			return false;
		}
		map<string, Base3DObject*>& objectsInRoom = currentRoom->GetObjectsInRoomRef();
		BOOL canSeePlayer = true;
		float distanceToHitPoint = 0.f;
		for (map<string, Base3DObject*>::value_type& objectInRoom : objectsInRoom)
		{
			if ((*objectInRoom.second->GetColliderCube().begin()).second->isIntersectRay(mPos, sightRayDir, &distanceToHitPoint))
			{
				if (distanceToHitPoint > 0 && distanceToHitPoint < distanceToPlayer)
				{
					canSeePlayer = false;
					break;
				}
			}
		}
		if (canSeePlayer == true)
		{
			if(outPlayerPos != nullptr)
			{
				*outPlayerPos = playerPos;
			}
			return true;
		}
	}
	return false;
}

void Chaser::Update()
{
	/*if(GetTickCount() - mLastUpdateTime > mCycleTime)
	{
		mLastUpdateTime = GetTickCount();
		Player* player = mRoomCenter->GetPlayer();
		
		vector<D3DXPLANE> sightFrustum;
		MakeSightFrustum(sightFrustum);
		if(ObjectInSightFrustum(player, sightFrustum))
		{
			D3DXVECTOR3 playerPos = player->GetPos();
			D3DXVECTOR3 sightRayDir = playerPos - mPos;
			float distanceToPlayer = D3DXVec3Length(&sightRayDir);
			D3DXVec3Normalize(&sightRayDir, &sightRayDir);
			map<string, Base3DObject*>& objectsInRoom = mRoomCenter->FindRoomIncludePos(mPos)->GetObjectsInRoomRef();
			BOOL canSeePlayer = true;
			float distanceToHitPoint = 0.f;
			for (map<string, Base3DObject*>::value_type& objectInRoom : objectsInRoom)
			{
				if((*objectInRoom.second->GetColliderCube().begin()).second->isIntersectRay(mPos, sightRayDir, &distanceToHitPoint))
				{
					if(distanceToHitPoint > 0 && distanceToHitPoint < distanceToPlayer)
					{
						canSeePlayer = false;
						break;
					}
				}
			}
			if(canSeePlayer == true)
			{
				if(distanceToPlayer < attackRange)
				{
					Attack(player);
				}
				else
				{
					SetTarget(playerPos);
				}
			}
		}
		else
		{
			if(mChaserState != ChaserState::IDLE && mPath.empty() == true)
			{
				ReturnToBasePos();
			}
		}
	}

	if(mPath.empty() == false)
	{
		FollowingPath();
	}*/
	if(ChaserState * newChaserState =  mChaserState->Update(this))
	{
		SAFE_DELETE(mChaserState);
		newChaserState->Enter(this);
		mChaserState = newChaserState;
	}
	mSkinnedMesh->Update();
	Base3DObject::Update();
}

void Chaser::Render()
{
	Base3DObject::Render();
	mSkinnedMesh->Render(nullptr);
}
