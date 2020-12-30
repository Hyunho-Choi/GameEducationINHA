#pragma once
#include "ColliderPolygon.h"

class ColliderCube : public ColliderPolygon
{
public:
	ColliderCube();
	virtual ~ColliderCube();
	void Setup() override;
	void Update() override;
	void Render() override;
	
	void SetCubeCollider(float height, float width, float depth);
	
	static bool IsCollision(ColliderCube* obj1Cube, ColliderCube* obj2Cube);

	D3DXVECTOR3* GetAxisDir();

	float GetCubeHeight();
	float GetCubeWidth();
	float GetCubeDepth();

	static D3DXVECTOR3 CheckCollidePosNormal(ColliderCube* obj1Cube, ColliderCube* obj2Cube);
	static bool isIntersect(pair<D3DXVECTOR3, D3DXVECTOR3> line1, pair<D3DXVECTOR3, D3DXVECTOR3> line2);
	static int ccw(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3);
	bool isIntersectRay(D3DXVECTOR3& rayPos, D3DXVECTOR3& rayDir, OUT float* distance);
private:
	float mCubeHeight;
	float mCubeWidth;
	float mCubeDepth;

	D3DXVECTOR3 m_vOriCenterPos;
	D3DXVECTOR3 m_vOriAxisDir[3];
	
	vector<D3DXVECTOR3> m_BoxDrawVertex;
	
	D3DXVECTOR3 m_vCenterPos;
	D3DXVECTOR3 m_vAxisDir[3];
	
	float m_fAxisLen[3];
	float m_fAxisHalfLen[3];

	D3DXVECTOR3* m_pCollidePosNormal;

	
};

