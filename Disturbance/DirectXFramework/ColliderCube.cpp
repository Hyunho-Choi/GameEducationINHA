#include "stdafx.h"
#include "ColliderCube.h"


ColliderCube::ColliderCube()
	: mCubeWidth(1.0f)
	, mCubeHeight(1.0f)
	, mCubeDepth(1.0f)
	, m_pCollidePosNormal(nullptr)
{
	
}

ColliderCube::~ColliderCube()
{
}

void ColliderCube::Setup()
{
	D3DXVECTOR3 mMax = D3DXVECTOR3 (mCubeWidth / 2, mCubeHeight / 2, mCubeDepth / 2);
	D3DXVECTOR3 mMin = D3DXVECTOR3(-mCubeWidth / 2, -mCubeHeight / 2, -mCubeDepth / 2);

	m_vOriCenterPos = (mMin + mMax) / 2.0f;

	m_vOriAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOriAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOriAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(mMax.x - mMin.x);
	m_fAxisLen[1] = fabs(mMax.y - mMin.y);
	m_fAxisLen[2] = fabs(mMax.z - mMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = (m_fAxisLen[1] / 2.0f) + 0.5f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	vector<D3DXVECTOR3> m_BoxVertex;

	m_BoxVertex.push_back(D3DXVECTOR3(-m_fAxisHalfLen[0], 0, -m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(-m_fAxisHalfLen[0], m_fAxisHalfLen[1], -m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(m_fAxisHalfLen[0], m_fAxisHalfLen[1], -m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(m_fAxisHalfLen[0], 0, -m_fAxisHalfLen[2]));

	m_BoxVertex.push_back(D3DXVECTOR3(-m_fAxisHalfLen[0], 0, m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(-m_fAxisHalfLen[0], m_fAxisHalfLen[1], m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(m_fAxisHalfLen[0], m_fAxisHalfLen[1], m_fAxisHalfLen[2]));
	m_BoxVertex.push_back(D3DXVECTOR3(m_fAxisHalfLen[0], 0, m_fAxisHalfLen[2]));

	vector<DWORD> BoxIndex;

	// ¾Õ¸é
	BoxIndex.push_back(0);
	BoxIndex.push_back(1);
	BoxIndex.push_back(1);
	BoxIndex.push_back(2);
	BoxIndex.push_back(2);
	BoxIndex.push_back(3);
	BoxIndex.push_back(3);
	BoxIndex.push_back(0);

	// µÞ¸é
	BoxIndex.push_back(4);
	BoxIndex.push_back(5);
	BoxIndex.push_back(5);
	BoxIndex.push_back(6);
	BoxIndex.push_back(6);
	BoxIndex.push_back(7);
	BoxIndex.push_back(7);
	BoxIndex.push_back(4);

	// À­¸é
	BoxIndex.push_back(0);
	BoxIndex.push_back(4);
	BoxIndex.push_back(1);
	BoxIndex.push_back(5);
	// ¹Ø¸é
	BoxIndex.push_back(2);
	BoxIndex.push_back(6);
	BoxIndex.push_back(3);
	BoxIndex.push_back(7);

	m_BoxDrawVertex.resize(BoxIndex.size());

	for (unsigned int i = 0; i < BoxIndex.size(); ++i)
	{
		m_BoxDrawVertex[i] = m_BoxVertex[BoxIndex[i]];
	}
}

void ColliderCube::Update()
{
	D3DXMATRIXA16 matR;
	D3DXMATRIXA16 matT;
	
	D3DXMatrixTranslation(&matT, (*mPosition).x, (*mPosition).y, (*mPosition).z);
	D3DXMatrixRotationQuaternion(&matR, mRot);

	D3DXMATRIXA16 m_matWorldTM = matR * matT;

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(
			&m_vAxisDir[i],
			&m_vOriAxisDir[i],
			&m_matWorldTM
		);
	}

	D3DXVec3TransformCoord(
		&m_vCenterPos,
		&m_vOriCenterPos,
		&m_matWorldTM);
	
}


void ColliderCube::SetCubeCollider(float height, float width, float depth)
{
	mCubeWidth = width;
	mCubeHeight = height;
	mCubeDepth = depth;
	Setup();
}

bool ColliderCube::IsCollision(ColliderCube * obj1Cube, ColliderCube * obj2Cube)
{
	float cos[3][3];
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;
	const float cutOff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = obj2Cube->m_vCenterPos - obj1Cube->m_vCenterPos;

	float obj1AxisHalfLen[3];
	float obj2AxisHalfLen[3];

	obj1AxisHalfLen[0] = obj1Cube->m_fAxisHalfLen[0];
	obj1AxisHalfLen[1] = obj1Cube->m_fAxisHalfLen[1];
	obj1AxisHalfLen[2] = obj1Cube->m_fAxisHalfLen[2];
	
	obj2AxisHalfLen[0] = obj2Cube->m_fAxisHalfLen[0];
	obj2AxisHalfLen[1] = obj2Cube->m_fAxisHalfLen[1];
	obj2AxisHalfLen[2] = obj2Cube->m_fAxisHalfLen[2];
	
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; ++b)
		{
			cos[a][b] = D3DXVec3Dot(&obj1Cube->m_vAxisDir[a], &obj2Cube->m_vAxisDir[b]);
			absCos[a][b] = abs(cos[a][b]);

			if (absCos[a][b] > cutOff)
				existsParallelPair = true;
		}
		dist[a] = D3DXVec3Dot(&obj1Cube->m_vAxisDir[a], &D);
		r = abs(dist[a]);

		r0 = obj1AxisHalfLen[a];
		r1 = obj2AxisHalfLen[0] * absCos[a][0] +
			obj2AxisHalfLen[1] * absCos[a][1] +
			obj2AxisHalfLen[2] * absCos[a][2];

		if (r > r0 + r1)
			return false;

	} // << : for a

	for (int b = 0; b < 3; ++b)
	{
		r = abs(D3DXVec3Dot(&obj2Cube->m_vAxisDir[b], &D));
		r0 = obj1AxisHalfLen[0] * absCos[0][b] +
			obj1AxisHalfLen[1] * absCos[1][b] +
			obj1AxisHalfLen[2] * absCos[2][b];
		r1 = obj2AxisHalfLen[b];

		if (r > r0 + r1)
		{
			return false;
		}
	}

	if (existsParallelPair) 
		return true;

	{
		// : 0
		r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
		r0 = obj1AxisHalfLen[0] * absCos[2][0] + obj1AxisHalfLen[2] * absCos[0][0];
		r1 = obj2AxisHalfLen[1] * absCos[1][2] + obj2AxisHalfLen[2] * absCos[1][1];
		if (r > r0 + r1) return false;

		r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
		r0 = obj1AxisHalfLen[0] * absCos[2][1] + obj1AxisHalfLen[2] * absCos[0][1];
		r1 = obj2AxisHalfLen[0] * absCos[1][2] + obj2AxisHalfLen[2] * absCos[1][0];
		if (r > r0 + r1) return false;

		r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
		r0 = obj1AxisHalfLen[0] * absCos[2][2] + obj1AxisHalfLen[2] * absCos[0][2];
		r1 = obj2AxisHalfLen[0] * absCos[1][1] + obj2AxisHalfLen[1] * absCos[1][0];
		if (r > r0 + r1) return false;

		// : 1
		r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
		r0 = obj1AxisHalfLen[0] * absCos[1][1] + obj1AxisHalfLen[1] * absCos[0][1];
		r1 = obj2AxisHalfLen[1] * absCos[2][2] + obj2AxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1) return false;

		r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
		r0 = obj1AxisHalfLen[0] * absCos[1][1] + obj1AxisHalfLen[1] * absCos[0][1];
		r1 = obj2AxisHalfLen[0] * absCos[2][2] + obj2AxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1) return false;

		r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
		r0 = obj1AxisHalfLen[0] * absCos[1][2] + obj1AxisHalfLen[1] * absCos[0][2];
		r1 = obj2AxisHalfLen[1] * absCos[2][1] + obj2AxisHalfLen[1] * absCos[2][0];
		if (r > r0 + r1) return false;

		// : 2
		r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
		r0 = obj1AxisHalfLen[1] * absCos[2][0] + obj1AxisHalfLen[2] * absCos[1][0];
		r1 = obj2AxisHalfLen[1] * absCos[0][2] + obj2AxisHalfLen[2] * absCos[0][1];
		if (r > r0 + r1) return false;

		r = abs(dist[2] * cos[1][1] - dist[1] * cos[1][1]);
		r0 = obj1AxisHalfLen[1] * absCos[2][1] + obj1AxisHalfLen[2] * absCos[1][1];
		r1 = obj2AxisHalfLen[0] * absCos[0][2] + obj2AxisHalfLen[2] * absCos[0][0];
		if (r > r0 + r1) return false;

		r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
		r0 = obj1AxisHalfLen[1] * absCos[2][2] + obj1AxisHalfLen[2] * absCos[1][2];
		r1 = obj2AxisHalfLen[0] * absCos[0][1] + obj2AxisHalfLen[1] * absCos[0][0];
		if (r > r0 + r1) return false;
	}
	return true;
}

D3DXVECTOR3 * ColliderCube::GetAxisDir()
{
	return m_vAxisDir;
}

float ColliderCube::GetCubeHeight()
{
	return mCubeHeight;
}

float ColliderCube::GetCubeWidth()
{
	return mCubeWidth;
}

float ColliderCube::GetCubeDepth()
{
	return mCubeDepth;
}

D3DXVECTOR3 ColliderCube::CheckCollidePosNormal(ColliderCube* obj1Cube, ColliderCube* obj2Cube)
{
	D3DXVECTOR3 rectObj1[4], rectObj2[4], result;

	rectObj1[0] = (*obj1Cube->GetPosition()
		+ (obj1Cube->GetAxisDir()[0] * (obj1Cube->GetCubeWidth() * 0.5f))
		+ (obj1Cube->GetAxisDir()[2] * (obj1Cube->GetCubeDepth() * 0.5f)));
	rectObj1[1] = (*obj1Cube->GetPosition()
		+ (obj1Cube->GetAxisDir()[0] * (obj1Cube->GetCubeWidth() * 0.5f))
		- (obj1Cube->GetAxisDir()[2] * (obj1Cube->GetCubeDepth() * 0.5f)));
	rectObj1[2] = (*obj1Cube->GetPosition()
		- (obj1Cube->GetAxisDir()[0] * (obj1Cube->GetCubeWidth() * 0.5f))
		- (obj1Cube->GetAxisDir()[2] * (obj1Cube->GetCubeDepth() * 0.5f)));
	rectObj1[3] = (*obj1Cube->GetPosition()
		- (obj1Cube->GetAxisDir()[0] * (obj1Cube->GetCubeWidth() * 0.5f))
		+ (obj1Cube->GetAxisDir()[2] * (obj1Cube->GetCubeDepth() * 0.5f)));

	rectObj2[0] = (*obj2Cube->GetPosition()
		+ (obj2Cube->GetAxisDir()[0] * (obj2Cube->GetCubeWidth() * 0.5f))
		+ (obj2Cube->GetAxisDir()[2] * (obj2Cube->GetCubeDepth() * 0.5f)));
	rectObj2[1] = (*obj2Cube->GetPosition()
		+ (obj2Cube->GetAxisDir()[0] * (obj2Cube->GetCubeWidth() * 0.5f))
		- (obj2Cube->GetAxisDir()[2] * (obj2Cube->GetCubeDepth() * 0.5f)));
	rectObj2[2] = (*obj2Cube->GetPosition()
		- (obj2Cube->GetAxisDir()[0] * (obj2Cube->GetCubeWidth() * 0.5f))
		- (obj2Cube->GetAxisDir()[2] * (obj2Cube->GetCubeDepth() * 0.5f)));
	rectObj2[3] = (*obj2Cube->GetPosition()
		- (obj2Cube->GetAxisDir()[0] * (obj2Cube->GetCubeWidth() * 0.5f))
		+ (obj2Cube->GetAxisDir()[2] * (obj2Cube->GetCubeDepth() * 0.5f)));


	vector<D3DXVECTOR3> vecPosInRect;
	for (int i = 0; i < 4; i++)
	{
		if ((abs(D3DXVec3Dot(&(rectObj1[i] - *obj2Cube->GetPosition()), &(obj2Cube->GetAxisDir()[0]))) <= obj2Cube->GetCubeWidth()) &&
			(abs(D3DXVec3Dot(&(rectObj1[i] - *obj2Cube->GetPosition()), &(obj2Cube->GetAxisDir()[2]))) <= obj2Cube->GetCubeDepth()))
		{
			vecPosInRect.push_back(rectObj1[i]);
		}
	}

	if (!vecPosInRect.empty())
	{
		for (int i = 0; i < 4; i++)
		{
			if (i < 3)
			{
				if (isIntersect({ *obj1Cube->GetPosition() ,vecPosInRect[0] }, { rectObj2[i] , rectObj2[i + 1] }))
				{
					D3DXVec3Cross(&result, &(rectObj2[i + 1] - rectObj2[i]), &D3DXVECTOR3(0, 1, 0));
					D3DXVec3Normalize(&result, &result);
					return result;
				}
			}
			else
			{
				if (isIntersect({ *obj1Cube->GetPosition() ,vecPosInRect[0] }, { rectObj2[i] , rectObj2[0] }))
				{
					D3DXVec3Cross(&result, &(rectObj2[0] - rectObj2[i]), &D3DXVECTOR3(0, 1, 0));
					D3DXVec3Normalize(&result, &result);
					return result;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if ((abs(D3DXVec3Dot(&(rectObj2[i] - *obj1Cube->GetPosition()), &(obj1Cube->GetAxisDir()[0]))) <= obj1Cube->GetCubeWidth()) &&
				(abs(D3DXVec3Dot(&(rectObj2[i] - *obj1Cube->GetPosition()), &(obj1Cube->GetAxisDir()[2]))) <= obj1Cube->GetCubeDepth()))
			{
				vecPosInRect.push_back(rectObj2[i]);
			}
			if (!vecPosInRect.empty())
			{
				for (int i = 0; i < 4; i++)
				{
					if (i < 3)
					{
						if (isIntersect({ *obj2Cube->GetPosition() ,vecPosInRect[0] }, { rectObj1[i] , rectObj1[i + 1] }))
						{
							D3DXVec3Cross(&result, &(rectObj1[i + 1] - rectObj1[i]), &D3DXVECTOR3(0, 1, 0));
							D3DXVec3Normalize(&result, &result);
							return result;
						}
					}
					else
					{
						if (isIntersect({ *obj2Cube->GetPosition() ,vecPosInRect[0] }, { rectObj1[i] , rectObj1[0] }))
						{
							D3DXVec3Cross(&result, &(rectObj1[0] - rectObj1[i]), &D3DXVECTOR3(0, 1, 0));
							D3DXVec3Normalize(&result, &result);
							return result;
						}
					}
				}
			}
		}
	}
	return D3DXVECTOR3(0, 0, 0);
}

bool ColliderCube::isIntersect(pair<D3DXVECTOR3, D3DXVECTOR3> line1, pair<D3DXVECTOR3, D3DXVECTOR3> line2)
{
	D3DXVECTOR3 a = line1.first;
	D3DXVECTOR3 b = line1.second;
	D3DXVECTOR3 c = line2.first;
	D3DXVECTOR3 d = line2.second;
	int ab = ccw(a, b, c) * ccw(a, b, d);
	int cd = ccw(c, d, a) * ccw(c, d, b);
	if (ab == 0 && cd == 0) {
		if (a > b)swap(a, b);
		if (c > d)swap(c, d);
		return c <= b&&a <= d;
	}
	return ab <= 0 && cd <= 0;
}

int ColliderCube::ccw(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3)
{
	int op = point1.x * point2.z + point2.x * point3.z + point3.x * point1.z;
	op -= (point1.z * point2.x + point2.z * point3.x + point3.z * point1.x);
	if (op > 0)return 1;
	else if (op == 0)return 0;
	else return -1;
}

bool ColliderCube::isIntersectRay(D3DXVECTOR3& rayPos, D3DXVECTOR3& rayDir, OUT float* distance)
{
	D3DXMATRIXA16 matR, matT, matInverseWorld;
	D3DXMatrixRotationQuaternion(&matR, mRot);
	D3DXMatrixTranslation(&matT, mPosition->x, mPosition->y, mPosition->z);
	matInverseWorld = matR * matT;
	D3DXMatrixInverse(&matInverseWorld, nullptr, &matInverseWorld);
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &rayPos, &matInverseWorld);
	D3DXVec3TransformNormal(&localRayDir, &rayDir, &matInverseWorld);

	D3DXVECTOR3 min = {-mCubeWidth / 2, -mCubeHeight /2, -mCubeDepth / 2};
	D3DXVECTOR3 max = { mCubeWidth / 2, mCubeHeight / 2, mCubeDepth / 2 };
	
	float tmin = (min.x - localRayPos.x) / localRayDir.x;
	float tmax = (max.x - localRayPos.x) / localRayDir.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - localRayPos.y) / localRayDir.y;
	float tymax = (max.y - localRayPos.y) / localRayDir.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - localRayPos.z) / localRayDir.z;
	float tzmax = (max.z - localRayPos.z) / localRayDir.z;

	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	if(distance != nullptr)
	{
		*distance = tmin;
	}
	
	return true;
}

void ColliderCube::Render()
{
	D3DXMATRIXA16 matS, matR, matT, stockMat, matWorld;
	D3DXMatrixScaling(&matS, 1, 1, 1);
	D3DXMatrixRotationQuaternion(&matR, mRot);
	D3DXMatrixTranslation(&matT, mPosition->x, mPosition->y, mPosition->z);

	gD3Device->GetTransform(D3DTS_WORLD, &stockMat);
	gD3Device->SetTransform(D3DTS_WORLD, &(matS * matR * matT));

	gD3Device->SetRenderState(D3DRS_LIGHTING, false);
	gD3Device->SetTexture(0, nullptr);
	gD3Device->SetFVF(D3DFVF_XYZ);
	//gD3Device->DrawPrimitiveUP(D3DPT_LINELIST, m_BoxDrawVertex.size() / 2, &m_BoxDrawVertex[0], sizeof(D3DXVECTOR3));

	gD3Device->SetTransform(D3DTS_WORLD, &stockMat);
}

