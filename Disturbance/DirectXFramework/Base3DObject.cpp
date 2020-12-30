#include "stdafx.h"
#include "Base3DObject.h"
#include "ColliderSphere.h"
#include "ColliderCube.h"
#include "CollideHandle.h"

Base3DObject::Base3DObject()
	: mScale(1, 1, 1)
	, mPos(0, 0, 0)
	, mbIsInteractable(false)
	, mStateTag(eStateTag::NON_STATE)
	, mShaderPath(eShaderPath::ALL_PATH)
	, mCullingSize(0.f)
{
	D3DXQuaternionIdentity(&mRot);
	CollideHandle = DefaultColliderHandler;
	mColliderSphere = new ColliderSphere;
	mColliderSphere->SetPosition(&mPos);
	mColliderSphere->SetRotation(&mRot);
	mColliderSphere->SetScale(&mScale);
}

Base3DObject::~Base3DObject()
{
	SAFE_DELETE(mColliderSphere);
	
	for (auto& element : mColliderCubeMap)
	{
		SAFE_DELETE(element.second);
	}

	mColliderCubeMap.clear();
}

void Base3DObject::Update()
{
	mColliderSphere->Update();
	for (map<string, ColliderCube*>::iterator it = mColliderCubeMap.begin(); it != mColliderCubeMap.end(); ++it)
	{
		it->second->Update();
	}

}

void Base3DObject::Setup()
{
	mColliderSphere->Setup();
	for (map<string, ColliderCube*>::iterator it = mColliderCubeMap.begin(); it != mColliderCubeMap.end(); ++it)
	{
		it->second->Setup();
	}
}

void Base3DObject::Render()
{
	D3DXMATRIXA16 matWorld, matS, matR, matT;

	D3DXMatrixScaling(&matS, Base3DObject::mScale.x, Base3DObject::mScale.y, Base3DObject::mScale.z);
	D3DXMatrixRotationQuaternion(&matR, &mRot);
	D3DXMatrixTranslation(&matT, Base3DObject::mPos.x, Base3DObject::mPos.y, Base3DObject::mPos.z);
	matWorld = matS * matR * matT;

	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);
	
	for (map<string, ColliderCube*>::iterator it = mColliderCubeMap.begin(); it != mColliderCubeMap.end(); ++it)
	{
		it->second->Render();
	}
	
}

//void Base3DObject::SetObjectTag()
//{
//	string objectName = this->GetObjectName();
//
//	if (objectName == "player")
//	{
//		mObjectTag = eObjTag::PLAYER_CHARACTER;
//	}
//	else if (objectName == "key")
//	{
//		mObjectTag = eObjTag::STATIC_OBJECT;
//	}
//	else if (objectName == "door")
//	{
//		mObjectTag = eObjTag::STATIC_OBJECT;
//	}
//	else if (objectName == "desk")
//	{
//		mObjectTag = eObjTag::STATIC_OBJECT;
//	}
//}

void Base3DObject::AddColliderCube(string colliderName)
{
	ColliderCube* colliderCube = new ColliderCube;
	colliderCube->SetPosition(&mPos);
	colliderCube->SetRotation(&mRot);
	colliderCube->SetScale(&mScale);
	mColliderCubeMap.insert(make_pair(colliderName, colliderCube));
}

void Base3DObject::DeleteColliderCube(string colliderName)
{
	mColliderCubeMap.erase(mColliderCubeMap.find(colliderName));
}

void Base3DObject::SetPos(const D3DXVECTOR3& pos)
{
	mPos = pos;
}

D3DXVECTOR3 Base3DObject::GetPos()
{
	return mPos;
}

void Base3DObject::SetScale(const D3DXVECTOR3& scale)
{
	mScale = scale;
}

D3DXVECTOR3 Base3DObject::GetScale()
{
	return mScale;
}


void Base3DObject::LoadColliderCube()
{

	//gJSON->WriteJSON("./Load/ColliderCube.json");

	if( "player" == this->GetObjectName())
	{
		
	}
}

void Base3DObject::LoadColliderSphere()
{
	
}

void Base3DObject::SetRot(const D3DXQUATERNION& rot)
{
	mRot = rot;
}
D3DXQUATERNION Base3DObject::GetRot()
{
	return mRot;
}
