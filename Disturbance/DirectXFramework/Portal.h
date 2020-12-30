#pragma once
#include "Base3DObject.h"


class Portal :
	public Static3DObject
{
	static const D3DXVECTOR3 NOT_COLLIDED_POS;
	D3DXVECTOR3 mExitPos;
	D3DXVECTOR3 mPortalDir;
	D3DXVECTOR3 mPortalLineDir;
	map<string, pair<Base3DObject *, D3DXVECTOR3>> mObjectsInPortal;
public:
	Portal(D3DXVECTOR3 portalDir);
	virtual ~Portal();

	void Render() override;
	
	void SetExitPos(D3DXVECTOR3 exitPos) { mExitPos = exitPos; }
	D3DXVECTOR3 GetExitPos() const { return mExitPos; }
	void PortalColliderHandler(Base3DObject * myObject, string& myColliderTag, Base3DObject * otherObject, string& otherColliderTag);
};


