#pragma once
#include "ChaserState.h"
class AttackingChaserState :
	public ChaserState
{
private :
	vector<D3DXVECTOR3> mPath;
	D3DXVECTOR3 mLastFindedPlayerPos;
	bool mbMakePath;
	float mSpeed;
protected:
public :
	AttackingChaserState();
	~AttackingChaserState();

	void FollowingPath();
	
	void Enter(Chaser* chaser) override;
	ChaserState* Update(Chaser* chaser) override;
};

