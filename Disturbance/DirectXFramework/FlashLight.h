#pragma once
#include "Base3DObject.h"

class FlashLight :
	public Base3DObject
{
private:
	D3DXVECTOR3* mpPos;
	D3DXQUATERNION* mpRot;
	LPD3DXMESH	mpMesh;

public:
	FlashLight();
	virtual ~FlashLight();

	void Setup(D3DXVECTOR3* playerPos, D3DXQUATERNION* playerRot);
	void Update() override;
	void Render() override;

	void RenderMesh();
};

