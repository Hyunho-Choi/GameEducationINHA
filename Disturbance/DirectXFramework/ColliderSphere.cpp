#include "stdafx.h"
#include "ColliderSphere.h"


ColliderSphere::ColliderSphere()
	: mRadius(1.0f)
{
	D3DXCreateSphere(gD3Device, mRadius, 10, 10, &mMesh, nullptr);
}


ColliderSphere::~ColliderSphere()
{
	SAFE_RELEASE(mMesh);
}

void ColliderSphere::Setup()
{
}

void ColliderSphere::Update()
{
	
}

void ColliderSphere::Render()
{
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, mPosition->x, mPosition->y, mPosition->z);
	gD3Device->SetTexture(0, nullptr);
	gD3Device->SetTransform(D3DTS_WORLD, &matT);
	mMesh->DrawSubset(0);
}

void ColliderSphere::SetRadius(float radius)
{
	mRadius = radius;

	SAFE_RELEASE(mMesh);
	D3DXCreateSphere(gD3Device, mRadius, 10, 10, &mMesh, nullptr);
}
