#include "stdafx.h"
#include "CullingManager.h"
#include "ColliderCube.h"

void CullingManager::Setup()
{
	// 생성시 필요한 것
	float one = 1.0f;
	mVecFrustumPlanes.resize(6);
	mVecOriFrustumCube.resize(8);
	mVecWorldFrustumCube.resize(8);

	mVecOriFrustumCube[0] = D3DXVECTOR3(-one, -one, 0);
	mVecOriFrustumCube[1] = D3DXVECTOR3(-one,  one, 0);
	mVecOriFrustumCube[2] = D3DXVECTOR3( one,  one, 0);
	mVecOriFrustumCube[3] = D3DXVECTOR3( one, -one, 0);

	mVecOriFrustumCube[4] = D3DXVECTOR3(-one, -one, 1);
	mVecOriFrustumCube[5] = D3DXVECTOR3(-one,  one, 1);
	mVecOriFrustumCube[6] = D3DXVECTOR3( one,  one, 1);
	mVecOriFrustumCube[7] = D3DXVECTOR3( one, -one, 1);
}

void CullingManager::SetFrustum()
{
	// 총업데이트에 한번 필요
	gD3Device->GetTransform(D3DTS_PROJECTION, &matProj);
	gD3Device->GetTransform(D3DTS_VIEW, &matView);

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3Unproject(
			&mVecWorldFrustumCube[i],
			&mVecOriFrustumCube[i],
			nullptr,
			&matProj,
			&matView,
			nullptr
		);
	}

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[0],
		&mVecWorldFrustumCube[0],
		&mVecWorldFrustumCube[1],
		&mVecWorldFrustumCube[2]
		);

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[1],
		&mVecWorldFrustumCube[6],
		&mVecWorldFrustumCube[5],
		&mVecWorldFrustumCube[4]
	);

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[2],
		&mVecWorldFrustumCube[1],
		&mVecWorldFrustumCube[5],
		&mVecWorldFrustumCube[6]
	);

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[3],
		&mVecWorldFrustumCube[0],
		&mVecWorldFrustumCube[3],
		&mVecWorldFrustumCube[7]
	);

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[4],
		&mVecWorldFrustumCube[1],
		&mVecWorldFrustumCube[0],
		&mVecWorldFrustumCube[4]
	);

	D3DXPlaneFromPoints(
		&mVecFrustumPlanes[5],
		&mVecWorldFrustumCube[2],
		&mVecWorldFrustumCube[6],
		&mVecWorldFrustumCube[7]
	);

}

bool CullingManager::CheckFrustumCulling(Base3DObject* obj)
{
	// 개별 업데이트마다 필요한 것
	for each(D3DXPLANE p in mVecFrustumPlanes)
	{
		if(D3DXPlaneDotCoord(&p, &obj->GetPos()) > obj->GetCullingSize())
			return false;
	}
	return true;
}
