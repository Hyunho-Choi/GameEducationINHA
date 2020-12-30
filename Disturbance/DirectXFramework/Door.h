#pragma once
#include "Static3DObject.h"
class Door :
	public Static3DObject
{
public:
	Door();
	virtual ~Door();

	void Setup(
	IN string	folder,
	IN string	fileName,
	eTypeTag	typeTag = eTypeTag::ITEM);

	virtual void Update() override;
	virtual void Render() override;
	
};

