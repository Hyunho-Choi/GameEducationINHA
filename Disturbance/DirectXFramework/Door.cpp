#include "stdafx.h"
#include "Door.h"


Door::Door()
{
}


Door::~Door()
{
}

void Door::Setup(string folder, string fileName, eTypeTag typeTag)
{
	Static3DObject::Setup(folder, fileName, typeTag);
}

void Door::Update()
{
	D3DXMATRIXA16 matSubWolrd, matRot, matTrans;
	D3DXMatrixTranslation(&matTrans, 0,0,0);
	D3DXMatrixRotationY(&matRot, 0.01f);

	matSubWolrd = matTrans * matRot;
	D3DXVec3TransformCoord(&mPos, &mPos, &matSubWolrd);
}

void Door::Render()
{
	Static3DObject::Render();
}
