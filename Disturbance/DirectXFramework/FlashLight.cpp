#include "stdafx.h"
#include "FlashLight.h"


FlashLight::FlashLight()
{
}


FlashLight::~FlashLight()
{
	SAFE_RELEASE(mpMesh);
}

void FlashLight::Setup(D3DXVECTOR3* playerPos, D3DXQUATERNION* playerRot)
{
	mpPos = playerPos;
	mpRot = playerRot;
	DWORD temp;
	//gModelLoader->LoadXfile("Resources/XFile/", "newFloor.X", mpMesh, nullptr, nullptr, temp);

	D3DXLoadMeshFromXA(
		"Resources/XFile/FlashLight.X",
		D3DXMESH_MANAGED,
		gD3Device,
		NULL,
		nullptr,
		NULL,
		nullptr,
		&mpMesh
	);
}

void FlashLight::Update()
{
	BaseObject::Update();
}

void FlashLight::Render()
{
	BaseObject::Render();
	D3DXMATRIXA16 matWorld, matS, matR, matT, matOffset;

	D3DXMatrixScaling(&matS,5, 1, 6);
	D3DXMatrixRotationQuaternion(&matR, mpRot);
	D3DXMatrixTranslation(&matT, mpPos->x, mpPos->y, mpPos->z);
	D3DXMatrixTranslation(&matOffset, 0, 0, -1);
	matWorld = matOffset * matS * matR * matT ;

	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);
	gShader->GetInstance()->RenderWithFlashShader(bind(&FlashLight::RenderMesh, this));

	//mpMesh->DrawSubset(0);
}

void FlashLight::RenderMesh()
{
	mpMesh->DrawSubset(0);
}
