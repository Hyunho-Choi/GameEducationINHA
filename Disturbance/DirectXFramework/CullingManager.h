#pragma once
#include "Singleton.h"

#define gCullingManager CullingManager::GetInstance()

class CullingManager :
	public Singleton<CullingManager>
{
private:
	vector<D3DXVECTOR3> mVecOriFrustumCube;
	vector<D3DXVECTOR3> mVecWorldFrustumCube;
	vector<D3DXPLANE> mVecFrustumPlanes;
	D3DXMATRIXA16 matView, matProj;

public:

	void Setup();
	void SetFrustum();
	bool CheckFrustumCulling(Base3DObject* obj);


};

