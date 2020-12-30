#include "stdafx.h"
#include "Player.h"
#include "UIImage.h"
#include "Scene.h"
#include "Inventory.h"

//
//Player::Inventory::Inventory()
//	: mKey(nullptr)
//	, mbGetKey(false)
//	, mInventory1(nullptr)
//	, mInventory2(nullptr)
//	, mInventory3(nullptr)
//{
//	Setup();
//}
//
//Player::Inventory::~Inventory()
//{
//	SAFE_DELETE(mKey);
//	SAFE_DELETE(mInventory1);
//	SAFE_DELETE(mInventory2);
//	SAFE_DELETE(mInventory3);
//}
//
//void Player::Inventory::Setup()
//{
//	mKey = new UIImage(string("Resources/Item/key.png"));
//	mKey->SetObjectName("key");
//	mKey->SetPos(D3DXVECTOR3(mKey->GetPos().x + mKey->GetWidth()*2.8f, mKey->GetPos().y + 15.f, 0));
//}
//
//void Player::Inventory::DrawInventoryUI(Scene& scene)
//{
//	mInventory1 = new UIImage("Resources/UI/Inventory.png");
//	mInventory1->SetObjectName("inventory");
//	mInventory1->SetPos(D3DXVECTOR3(mInventory1->GetPos().x,mInventory1->GetPos().y, 0.0f));
//	mInventory1->SetVisible(true);
//	scene.mGameUIs.insert(make_pair("inventory", mInventory1));
//
//	mInventory2 = new UIImage("Resources/UI/Inventory.png");
//	mInventory2->SetObjectName("inventory2");
//	mInventory2->SetPos(D3DXVECTOR3(mInventory2->GetPos().x + mInventory2->GetWidth(),mInventory2->GetPos().y, 0.0f));
//	mInventory2->SetVisible(true);
//	scene.mGameUIs.insert(make_pair("inventory2", mInventory2));
//
//	mInventory3 = new UIImage("Resources/UI/Inventory.png");
//	mInventory3->SetObjectName("inventory3");
//	mInventory3->SetPos(D3DXVECTOR3(mInventory3->GetPos().x + mInventory3->GetWidth()*2.f,mInventory3->GetPos().y, 0.0f));
//	mInventory3->SetVisible(true);
//	scene.mGameUIs.insert(make_pair("inventory3", mInventory3));
//}
//
//
//void Player::Inventory::Update()
//{
//	if(!mbGetKey)
//	{
//		mKey->SetVisible(false);
//	}
//	else
//	{
//		mKey->SetVisible(true);
//	}
//
//
//
//}
//
//void Player::Inventory::Render()
//{
//	if (mKey)
//	{
//		mKey->Render();
//	}
//
//}
//
//
//void Player::Inventory::SetInItem(BOOL state, string ObjectName)
//{
//	if (ObjectName == "key")
//	{
//		mbGetKey = state;
//	}
//
//}
//
//bool Player::Inventory::GetInItem(string ObjectName)
//{
//	if (ObjectName == "key")
//	{
//		return mbGetKey;
//	}
//
//}
//
//void Player::Inventory::UseItem(Base3DObject& Object)
//{
//	if (Object.GetTypeTag() == eTypeTag::DOOR &&  GetInItem("key"))
//	{
//		Object.SetStateTag(eStateTag::DOOR_OPEN);
//		SetInItem(false, "key");
//	}
//}

Inventory::Inventory()
{
	mUISlotImage[Recovery] = new UIImage("Resources/UI/Item.png", {1, 1}, 4, 4);
	mUISlotImage[Recovery]->SetObjectName("inventory");
	mUISlotImage[Recovery]->SetVisible(true);

	mUISlotImage[Useful] = new UIImage("Resources/UI/Item.png", {6, 1}, 4, 4);
	mUISlotImage[Useful]->SetObjectName("inventory2");
	mUISlotImage[Useful]->SetVisible(true);

	mUISlotImage[Key] = new UIImage("Resources/UI/Item.png", {11, 1}, 4, 4);
	mUISlotImage[Key]->SetObjectName("inventory3");
	mUISlotImage[Key]->SetVisible(true);

	mItemUIImageMap["key1"] = new UIImage("Resources/item/goodKey.png", {0, 0}, 4, 4);
	mItemUIImageMap["key1"]->SetObjectName("key1UIImage");

	mItemUIImageMap["key2"] = new UIImage("Resources/item/SecurityCard.png", { 0, 0 }, 4, 4);
	mItemUIImageMap["key2"]->SetObjectName("key2UIImage");
}

Inventory::~Inventory()
{
	for (int i = 0; i < SLOT_COUNT; ++i)
	{
		SAFE_DELETE(mUISlotImage[i]);
	}
}

void Inventory::Setup()
{
}

void Inventory::Update()
{
}

void Inventory::Render()
{
	for (int i = 0; i < SLOT_COUNT; ++i)
	{
		mUISlotImage[i]->Render();
		if(mSlotItem[i] != nullptr)
		{
			mItemUIImageMap[mSlotItem[i]->GetObjectName()]->SetPos(mUISlotImage[i]->GetPos());
			mItemUIImageMap[mSlotItem[i]->GetObjectName()]->SetVisible(true);
			mItemUIImageMap[mSlotItem[i]->GetObjectName()]->Render();
		}
	}
}

void Inventory::AddItem(eInventorySlot slot, Base3DObject* item)
{
	if(mSlotItem[slot] != nullptr)
	{
		assert("Inventory AddItem\nAlready Item in Slot");
		return;
	}
	mSlotItem[slot] = item;
}

void Inventory::UseItem(eInventorySlot slot)
{
	if (mSlotItem[slot] == nullptr)
	{
		assert("Inventory USeItem Error\nNo Item in Slot");
		return;
	}
	SAFE_DELETE(mSlotItem[slot]);
	mSlotItem[slot] = nullptr;
}

bool Inventory::HasItem(eInventorySlot slot, string itemName)
{
	if(mSlotItem[slot] != nullptr && mSlotItem[slot]->GetObjectName() == itemName)
	{
		return true;
	}
	return false;
}

