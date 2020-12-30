#include "stdafx.h"
#include "CCTV.h"

#include "ColliderSphere.h"
#include "SkinnedMesh.h"
#include "Camera.h"
#include "Chaser.h"

void CCTV::RenderDetectRangeMesh()
{
	D3DXMATRIXA16 matS, matT, matWorld;
	D3DXMatrixScaling(&matS, mDetectSphere->GetRadius(), mDetectSphere->GetRadius(), mDetectSphere->GetRadius());
	D3DXMatrixTranslation(&matT, mDetectSpherePos.x, mDetectSpherePos.y + 0.0001f, mDetectSpherePos.z);
	matWorld = matS * matT;
	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);

	gShader->GetInstance()->RenderWithCCTVShader(bind(&CCTV::RenderMesh, this));
	//mDetectRangeMesh->DrawSubset(0);

	//for (int i = 1; i < 3; ++i)
	//{
	//	D3DXMatrixScaling(&matS, mDetectSphere->GetRadius(), mDetectSphere->GetRadius(), mDetectSphere->GetRadius());
	//	D3DXMatrixTranslation(&matT, mDetectSphereRange[i].x, mDetectSphereRange[i].y + 0.0001f, mDetectSphereRange[i].z);
	//	matWorld = matS * matT;
	//	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);
	//	mDetectRangeMesh->DrawSubset(0);
	//}
}

void CCTV::RenderMesh()
{
	mDetectRangeMesh->DrawSubset(0);
}

void CCTV::Rotate()
{
	mRotateProgressTime += gTimeManager->GetDeltaTime() / 4.f;
	if(mRotateProgressTime > 1.f)
	{
		if(mRotateProgressTime > 1.5f)
		{
			mRotateProgressTime -= 1.5f;
			mbRotateRight = !mbRotateRight;
		}
		else
		{
			return;
		}
	}
	
	if(mbRotateRight)
	{
		D3DXVec3CatmullRom(&mDetectSpherePos, &mDetectSphereRange[0], &mDetectSphereRange[1], &mDetectSphereRange[2], &mDetectSphereRange[3], mRotateProgressTime);
		//mDetectSpherePos = BezierSecond(, mDetectSphereRange[1], mDetectSphereRange[2], mRotateProgressTime);
	}
	else
	{	
		D3DXVec3CatmullRom(&mDetectSpherePos, &mDetectSphereRange[3], &mDetectSphereRange[2], &mDetectSphereRange[1], &mDetectSphereRange[0], mRotateProgressTime);
	}
}

bool CCTV::DetectedTarget()
{
	if(D3DXVec3Length(&(*gCameraManager->GetCurrentCamera()->GetTarget() - mDetectSpherePos)) < mDetectSphere->GetRadius())
	{
		return true;
	}
	return false;
}

CCTV::CCTV(const LPD3DXMESH& floorMesh, D3DXVECTOR3 eyeDir, D3DXVECTOR3 pos)
	: mbRotateRight(true)
	, mbFindTarget(false)
	, mRotateProgressTime(0.f)
{
	SetScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
	mSkinnedMesh = new SkinnedMesh("Resources/XFile/CCTV", "CCTV.X");
	mSkinnedMesh->SetAnimationIndexBlend(0);
	D3DXVec3Normalize(&mEyeDir, &eyeDir);
	mPos = pos;
	BOOL isHit;
	float hitDist = 0;
	static D3DXVECTOR3 floorPos = D3DXVECTOR3(75, 0, 75);

	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, D3DX_PI / -2.f);
	D3DXVec3TransformNormal(&mEyeDir, &eyeDir, &matR);
	D3DXIntersect(floorMesh, &(mPos - floorPos), &mEyeDir, &isHit, nullptr, nullptr, nullptr, &hitDist, nullptr, nullptr);
	mDetectSphereRange[0] = mPos + mEyeDir * hitDist;
	
	D3DXMatrixRotationY(&matR, D3DX_PI / -6.f);
	D3DXVec3TransformNormal(&mEyeDir, &eyeDir, &matR);
	D3DXIntersect(floorMesh, &(mPos - floorPos), &mEyeDir, &isHit, nullptr, nullptr, nullptr, &hitDist, nullptr, nullptr);
	mDetectSphereRange[1] = mPos + mEyeDir * hitDist;

	D3DXMatrixRotationY(&matR, D3DX_PI / 6.f);
	D3DXVec3TransformNormal(&mEyeDir, &eyeDir, &matR);
	D3DXIntersect(floorMesh, &(mPos - floorPos), &mEyeDir, &isHit, nullptr, nullptr, nullptr, &hitDist, nullptr, nullptr);
	mDetectSphereRange[2] = mPos + mEyeDir * hitDist;
	
	D3DXMatrixRotationY(&matR, D3DX_PI / 2.f);
	D3DXVec3TransformNormal(&mEyeDir, &eyeDir, &matR);
	D3DXIntersect(floorMesh, &(mPos - floorPos), &mEyeDir, &isHit, nullptr, nullptr, nullptr, &hitDist, nullptr, nullptr);
	mDetectSphereRange[3] = mPos + mEyeDir * hitDist;

	mDetectSpherePos = mDetectSphereRange[0];
	mDetectSphere = new ColliderSphere;
	mDetectSphere->SetPosition(&mDetectSpherePos);
	mDetectSphere->SetRadius(5.5f);
	
	D3DXLoadMeshFromXA(
		"Resources/XFile/FlashLight.X",
		D3DXMESH_MANAGED,
		gD3Device,
		NULL,
		nullptr,
		NULL,
		nullptr,
		&mDetectRangeMesh
	);
	AddColliderCube("basicColliderCube");
	Base3DObject::Setup();
}

CCTV::~CCTV()
{
	SAFE_DELETE(mSkinnedMesh);
	SAFE_DELETE(mDetectSphere);
	SAFE_RELEASE(mDetectRangeMesh);
}

void CCTV::Update()
{
	if(DetectedTarget() == true)
	{
		if (mbFindTarget == false)
		{
			mbFindTarget = true;
			for (Chaser* chaser : mChasers)
			{
				chaser->SetTarget(*gCameraManager->GetCurrentCamera()->GetTarget());
			}
		}
	}
	else
	{
		if (mbFindTarget == true)
		{
			mbFindTarget = false;
		}
		Rotate();
	}
	
	mSkinnedMesh->Update();
	Base3DObject::Update();
}

void CCTV::Render()
{
	Base3DObject::Render();
	mSkinnedMesh->Render(nullptr);
	RenderDetectRangeMesh();
}
