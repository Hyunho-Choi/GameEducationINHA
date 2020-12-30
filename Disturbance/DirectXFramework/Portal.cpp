#include "stdafx.h"
#include "Portal.h"
#include "Player.h"
#include "ColliderCube.h"

const D3DXVECTOR3 Portal::NOT_COLLIDED_POS = D3DXVECTOR3(-999, -999, -999);

Portal::Portal(D3DXVECTOR3 portalDir)
	: mExitPos(0,0,0)
{
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, D3DX_PI * 0.5f);

	D3DXVECTOR3 portalLineDir;
	D3DXVec3TransformNormal(&portalLineDir, &portalDir, &matR);

	mPortalDir = portalDir;
	mPortalLineDir = portalLineDir;

	Setup("Resources/XFile/", "simplePlane.x");
	SetTypeTag(eTypeTag::PORTAL);

	mScale = D3DXVECTOR3(1, 2, 1);

	AddColliderCube("portalColliderCube");

	ColliderCube* portalColliderCube = mColliderCubeMap["portalColliderCube"];
	
	portalColliderCube->SetCubeCollider(portalColliderCube->GetCubeHeight() + mPortalLineDir.y,
										portalColliderCube->GetCubeWidth() + mPortalLineDir.x,
										portalColliderCube->GetCubeDepth() + mPortalLineDir.z);
}


Portal::~Portal()
{
}

void Portal::Render()
{
	D3DXVECTOR3 tempPos = mPos;
	mPos.y += mScale.y;
	Static3DObject::Render();
	mPos = tempPos;
	D3DXQUATERNION rotY;
	D3DXQuaternionRotationYawPitchRoll(&rotY, D3DX_PI, 0, 0);

	vector<string> notCollidedObjects;
	
	for (map<string, pair<Base3DObject*, D3DXVECTOR3>>::value_type& objectInPortal : mObjectsInPortal)
	{
		D3DXVECTOR3 prevPos = objectInPortal.second.first->GetPos();
		D3DXQUATERNION prevRot = objectInPortal.second.first->GetRot();
		
		objectInPortal.second.first->SetPos(objectInPortal.second.second);
		objectInPortal.second.first->SetRot(prevRot * rotY);
		
		objectInPortal.second.first->Render();

		if (objectInPortal.second.second == NOT_COLLIDED_POS)
		{
			notCollidedObjects.push_back(objectInPortal.first);
			objectInPortal.second.first->SetPos(prevPos);
			objectInPortal.second.first->SetRot(prevRot);
			continue;
		}

		D3DXVECTOR3 portalLine = mPortalLineDir;
		D3DXVECTOR3 objectToLine = prevPos - (mPos + mPortalLineDir);
		D3DXVECTOR3 crossVec;
		D3DXVec3Cross(&crossVec, &portalLine, &objectToLine);
		if(crossVec.y < 0)
		{
			notCollidedObjects.push_back(objectInPortal.first);
			objectInPortal.second.first->SetRot(prevRot);
			continue;
		}
		
		objectInPortal.second.first->SetPos(prevPos);
		objectInPortal.second.first->SetRot(prevRot);
		objectInPortal.second.second = NOT_COLLIDED_POS;
	}

	while(notCollidedObjects.empty() == false)
	{
		string objectName = notCollidedObjects.back();
		notCollidedObjects.pop_back();
		mObjectsInPortal.erase(mObjectsInPortal.find(objectName));
	}
}


void Portal::PortalColliderHandler(Base3DObject* myObject, string& myColliderTag, Base3DObject* otherObject, string& otherColliderTag)
{
	D3DXVECTOR3 exitPosCor = D3DXVECTOR3(otherObject->GetScale().x * -mPortalDir.x, otherObject->GetScale().y * -mPortalDir.y, otherObject->GetScale().z * -mPortalDir.z);
	
	if (mObjectsInPortal.find(otherObject->GetObjectName()) == mObjectsInPortal.end())
	{
		mObjectsInPortal.insert(make_pair(otherObject->GetObjectName(), make_pair(otherObject, mExitPos + exitPosCor + mPos - otherObject->GetPos())));
	}
	else
	{
		mObjectsInPortal[otherObject->GetObjectName()].second = mExitPos + exitPosCor + mPos - otherObject->GetPos();
	}
}
