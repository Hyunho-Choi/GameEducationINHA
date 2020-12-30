#include "stdafx.h"
#include "Item.h"

#include "UIBase.h"
#include "UIImage.h"


Item::Item()
	:mItemCount(NULL)
	, mUIImage(nullptr)
{
}


Item::~Item()
{
	SAFE_DELETE(mUIImage);
}

void Item::Setup()
{

	
	Static3DObject* eItem = new Static3DObject;
	eItem->SetObjectName("key");
	eItem->AddColliderCube("basicColliderCube");
	eItem->Setup("Resources/Item/", "Aimpoint2X.X");
	eItem->SetScale(D3DXVECTOR3(0.03f, 0.03f, 0.03f));
	eItem->SetPos(D3DXVECTOR3(8, 0, 14));
	eItem->SetRot(D3DXQUATERNION(0, 0.7f, 0, 1));
	eItem->SetIsInteractable(true);


}

void Item::Update()
{
}

void Item::Render()
{
}

void Item::SetCount(const int count)
{
	mItemCount = count;
}

int Item::GetCount() const
{
	return mItemCount;
}


void Item::SetPosition(const D3DXVECTOR3& pos)
{
	mUIImage->SetPos(pos);
}

void Item::SetInInventory(bool state)
{
	mUIImage->SetVisible(state);
}