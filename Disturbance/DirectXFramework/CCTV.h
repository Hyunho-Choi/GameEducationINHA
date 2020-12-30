#pragma once

#include "Base3DObject.h"

class SkinnedMesh;
class Static3DObject;
class ColliderSphere;
class Chaser;

class CCTV :
	public Base3DObject
{
private :
	bool mbRotateRight;
	bool mbFindTarget;
	float mRotateProgressTime;
	SkinnedMesh* mSkinnedMesh;
	D3DXVECTOR3 mEyeDir;
	D3DXVECTOR3 mDetectSpherePos;
	D3DXVECTOR3 mDetectSphereRange[4];
	LPD3DXMESH mDetectRangeMesh;
	vector<Chaser*> mChasers;
	ColliderSphere* mDetectSphere;

	void RenderDetectRangeMesh();
	void RenderMesh();
	void Rotate();
	bool DetectedTarget();
public :
	CCTV(const LPD3DXMESH& floorMesh, D3DXVECTOR3 eyeDir, D3DXVECTOR3 pos);
	~CCTV();

	void Update() override;
	void Render() override;
	

	void ConnectChaser(Chaser* chaser) { mChasers.push_back(chaser); }
};

