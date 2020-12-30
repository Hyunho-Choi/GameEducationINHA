#pragma once
#include "Static3DObject.h"


class UIImage;


class Item :
	public Static3DObject
{
private:

	int mItemCount;
	UIImage* mUIImage;
	
public:
	void Setup() override;
	void Update() override;
	void Render() override;
	
	void SetCount(const int count);
	int GetCount()const;

	void SetPosition(const D3DXVECTOR3& pos);

	void SetInInventory(bool state);

	Item();
	~Item();

};
