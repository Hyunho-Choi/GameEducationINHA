#pragma once
#include "ChaserState.h"
class MovingChaserState :
	public ChaserState
{
private :
	constexpr static float basicSpeed = 0.04f;
	constexpr static float angrySpeed = 8.5f;
	float mSpeed;
	vector<D3DXVECTOR3> mPath;
	void FindPath(D3DXVECTOR3& targetPos);
	void FollowingPath();
	void RotateToNextNode();
public :
	MovingChaserState();
	~MovingChaserState();

	void Enter(Chaser* chaser) override;
	ChaserState* Update(Chaser* chaser) override;
};

