#pragma once
#include "Base3DObject.h"
#include "Inventory.h"

class SkinnedMesh;
class CharacterState;
class UIImage;

class Player : public Base3DObject, public BaseObserver
{
	RECT mDrawFontArea;
	CharacterState* mCurState;
	D3DXVECTOR3 mMoveVelocity;
	Base3DObject * mInteractingObject;
	UIImage* mInteractingObjectMark;
	D3DXVECTOR3 mPrevPos;
	D3DXQUATERNION mPrevRot;
	Inventory mInventory;
	queue<CollisionEvent> mCollisionEventQueue;
	queue<CollisionEvent> mCollisionEventQueue2;

	void ChangeState(CharacterState * nextState);
	void DrawMark();
	void MoveBack();



public:
#pragma region GetterAndSetter
	D3DXQUATERNION& GetRotRef()
	{
		return mRot;
	}

	D3DXQUATERNION* GetRotPt()
	{
		return &mRot;
	}

	D3DXVECTOR3 GetPos() const
	{
		return mPos;
	}

	D3DXVECTOR3 * GetPosRef()
	{
		return &mPos;
	}

	void SetPos(const D3DXVECTOR3& pos)
	{
		mPos = pos;
	}

	void SetMoveVelocity(const D3DXVECTOR3& moveVelocity)
	{
		mMoveVelocity = moveVelocity;
	}
	void SetInteractingObject(Base3DObject* interactObject) { mInteractingObject = interactObject; }
	Base3DObject * GetInteractingObject() { return mInteractingObject; }
#pragma endregion

	SkinnedMesh* m_pSkinnedMesh;
	
	void Setup() override;
	void Update() override;
	void Render() override;
	bool Update(eEventName eventName, void* parameter) override;

	SkinnedMesh * GetSkinnedMesh()
	{
		return m_pSkinnedMesh;
	}

	void AddItem(eInventorySlot slot, Base3DObject* item);
	void UseItem(eInventorySlot slot);
	bool HasItem(eInventorySlot slot, string itemName);
	
	void SetAnimationSpeed(FLOAT spd);

	Player();
	virtual ~Player();

	void HandleInteractableObjectSphereCollideEvent(Base3DObject* interactableObject);
	void HandlePlayerCubeCollideEvent(Base3DObject* player, string& myColliderTag, Base3DObject * otherCollider, string& otherColliderTag);

	void ProcessCollisionEventQueue();
};

