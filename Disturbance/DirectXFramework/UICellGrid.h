#pragma once
#include "UIBase.h"

class UICellGrid :
    public UIBase
{
private:
	ID3DXLine* mLine;
	vector<D3DXVECTOR2> mHorizontalLineVertex;
	vector<D3DXVECTOR2> mVerticalLineVertex;
public :
	UICellGrid();
	~UICellGrid();
	
	void Render() override;
};

