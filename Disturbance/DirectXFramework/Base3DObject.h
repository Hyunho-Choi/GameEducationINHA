#pragma once
#include "BaseObject.h"
class ColliderCube;
class ColliderSphere;

enum class eObjTag { NON_OBJECT_TAG = 0, STATIC_OBJECT, INTERACTABLE_OBJECT };
enum class eTypeTag { PLAYER_CHARACTER, DOOR , WALL, ITEM, FLOOR, PORTAL};
enum class eStateTag { NON_STATE = 0, DOOR_OPEN, DOOR_CLOSE };

class Base3DObject :
	public BaseObject
{
public:

protected:
	D3DXVECTOR3 mScale;
	D3DXVECTOR3 mPos;
	D3DXQUATERNION mRot;
	eObjTag mObjectTag;
	BOOL mbIsInteractable;
	ColliderSphere* mColliderSphere;
	map<string,ColliderCube*> mColliderCubeMap;
	float mCullingSize;

	eStateTag mStateTag;
	eTypeTag mTypeTag;
	eShaderPath mShaderPath;

public:
	Base3DObject();
	virtual ~Base3DObject();

	virtual void Setup();
	virtual void Update();
	virtual void Render() override;

	void SetObjectTag(eObjTag tag) { mObjectTag = tag; }
	eObjTag GetObjectTag() const { return mObjectTag; }

	void SetTypeTag(eTypeTag tag) { mTypeTag = tag; }
	eTypeTag GetTypeTag() const { return mTypeTag; }

	void SetShaderPath(eShaderPath tag) { mShaderPath = tag; }
	eShaderPath GetShaderPath() const { return mShaderPath; }

	void SetCullingSize(float tag) { mCullingSize = tag; }
	float GetCullingSize() const { return mCullingSize; }


	void AddColliderCube(string colliderName);
	void DeleteColliderCube(string colliderName);

	void SetScale(const D3DXVECTOR3& scale);
	D3DXVECTOR3 GetScale();
	void SetRot(const D3DXQUATERNION& rotation);
	D3DXQUATERNION GetRot();
	void SetPos(const D3DXVECTOR3& pos);
	D3DXVECTOR3 GetPos();

	void LoadColliderCube();
	void LoadColliderSphere();
	
	function<void(Base3DObject *, string&, Base3DObject *, string&)> CollideHandle;
	function<void(string&)> DeleteInRoom;
	ColliderSphere* GetColliderSphere() { return mColliderSphere; }
	map<string, ColliderCube*>& GetColliderCube() { return mColliderCubeMap; }

	void SetIsInteractable(const BOOL isInteractable) { mbIsInteractable = isInteractable; }
	BOOL isInteractable() const { return mbIsInteractable; }
	void SetStateTag(const eStateTag statetag) { mStateTag = statetag; }

};

