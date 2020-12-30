#include "stdafx.h"
#include "UICellGrid.h"


UICellGrid::UICellGrid()
{
	float cellSize = 0;
	D3DVIEWPORT9 viewPort;
	gD3Device->GetViewport(&viewPort);
	cellSize = (float)viewPort.Width / HORIZONTAL_CELL_NUM;

	D3DXCreateLine(gD3Device, &mLine);
	mLine->SetWidth(3.f);
	mLine->SetAntialias(true);
	for (int i = 1; i < VERTICAL_CELL_NUM; ++i)
	{
		D3DXVECTOR2 pos(0, i * cellSize);
		mHorizontalLineVertex.emplace_back(pos);
		pos = { (float)viewPort.Width, (float)i * cellSize };
		mHorizontalLineVertex.emplace_back(pos);
	}

	for (int i = 1; i < HORIZONTAL_CELL_NUM; ++i)
	{
		mVerticalLineVertex.emplace_back(i * cellSize, 0);
		mVerticalLineVertex.emplace_back(i * cellSize, viewPort.Height);
	}
}

UICellGrid::~UICellGrid()
{
}

void UICellGrid::Render()
{
	if (mbVisible == false)
		return;
	D3DXVECTOR2 test[2];

	test[0] = D3DXVECTOR2(0, 30);
	test[1] = D3DXVECTOR2(1920, 30);
	
	mLine->Begin();
	for (int i = 0; i < mHorizontalLineVertex.size(); i += 2)
	{
		mLine->Draw(&mHorizontalLineVertex[i], 2, D3DCOLOR_XRGB(255, 0, 0));
	}
	//mLine->Draw(&mHorizontalLineVertex[mHorizontalLineVertex.size() * 0.5f + 1], 2, D3DCOLOR_XRGB(255, 255, 0));
	for (int i = 0; i < mVerticalLineVertex.size(); i += 2)
	{
		mLine->Draw(&mVerticalLineVertex[i], 2, D3DCOLOR_XRGB(255, 0, 0));
	}
	
	mLine->End();
}
