#pragma once
#include "Base3DObject.h"

class Base3DObject;

class ColliderPolygon
{
public:
	ColliderPolygon();
	virtual ~ColliderPolygon();

	virtual void Setup() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	D3DXVECTOR3 * GetPosition() { return mPosition; }
	
	void SetPosition(D3DXVECTOR3 * pos) { mPosition = pos; }
	D3DXVECTOR3* GetPosition() const { return mScale; }
	void SetRotation(D3DXQUATERNION * rot) { mRot = rot; }
	D3DXVECTOR3* GetRotation() const { return mScale; }
	void SetScale(D3DXVECTOR3 * scale) { mScale = scale; }
	D3DXVECTOR3* GetScale() const { return mScale; }

	LPD3DXMESH& GetMesh() { return mMesh; }
protected:
	LPD3DXMESH mMesh;
	D3DXVECTOR3 * mPosition;
	D3DXVECTOR3 * mScale;
	D3DXQUATERNION * mRot;
	
};