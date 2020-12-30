#pragma once

enum eInventorySlot
{
	Recovery = 0, Useful, Key, SLOT_COUNT
};

class Inventory
{
	UIImage * mUISlotImage[static_cast<int>(SLOT_COUNT)];
	Base3DObject* mSlotItem[static_cast<int>(SLOT_COUNT)];
	map<string, UIImage *> mItemUIImageMap;
public :
	Inventory();
	~Inventory();
	
	void Setup();
	void Update();
	void Render();

	void AddItem(eInventorySlot slot, Base3DObject* item);
	void UseItem(eInventorySlot slot);
	bool HasItem(eInventorySlot slot, string itemName);
	
};