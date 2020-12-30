#pragma once
#include "Base3DObject.h"

class ChaserState;
class SkinnedMesh;

class Chaser :
	public Base3DObject
{
private :
	static D3DXVECTOR3 baseSightDir;
	static float baseSightLength;
	static float baseSightAngle;
	static vector<D3DXPLANE> baseSightFrustum;
	
	RoomCenter* mRoomCenter;
	ChaserState* mChaserState;
	SkinnedMesh* mSkinnedMesh;
	vector<D3DXVECTOR3> mPath;
	D3DXVECTOR3 mBasePos;
	D3DXVECTOR3 mTargetPos;
	DWORD mCycleTime;
	DWORD mLastUpdateTime;
	float mSpeed;

	bool ObjectInSightFrustum(Base3DObject* object, vector<D3DXPLANE>& sightFrustum);
	void MakeSightFrustum(OUT vector<D3DXPLANE>& sightPlane);
	void FindPath(D3DXVECTOR3& endPos);
	void FollowingPath();
	void ReturnToBasePos();
	void Attack(Player* player);
public :
	constexpr static DWORD findCycleTime = 300;
	constexpr static DWORD attackCycleTime = 1500;
	constexpr static float attackRange = 1.f;
	constexpr static float findRange = 10.f;
	
	Chaser(D3DXVECTOR3 basePos, RoomCenter* roomCenter);
	virtual ~Chaser();

	D3DXVECTOR3 GetBasePos() const { return mBasePos; }
	SkinnedMesh* GetSkinnedMesh() const { return mSkinnedMesh; }

	void MatchTargetToPlayer();
	void SetTarget(D3DXVECTOR3& targetPos) { mTargetPos = targetPos; }
	D3DXVECTOR3 GetTarget() const { return mTargetPos; }

	bool IsPlayerInSight(OUT D3DXVECTOR3* outPlayerPos);
	void RotateToTarget();
	
	void Update() override;
	void Render() override;
};

