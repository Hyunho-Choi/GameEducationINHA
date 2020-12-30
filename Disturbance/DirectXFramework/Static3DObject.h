#pragma once
#include "Base3DObject.h"



class Static3DObject :
	public Base3DObject
{
private:
	LPD3DXMESH				mpMesh;
	D3DMATERIAL9*			mpMaterials;
	LPDIRECT3DTEXTURE9*		mpTextures;
	DWORD					mNumMaterials;
public:
	Static3DObject();
	virtual ~Static3DObject();

	void Setup(
		IN string	folder,
		IN string	fileName,
		eTypeTag	typeTag = eTypeTag::ITEM
	);

	LPD3DXMESH GetMesh() const { return mpMesh; }
	
	virtual void Update() override;
	virtual void Render() override;
	void RenderMesh();
	

	LPD3DXEFFECT LoadShader(const char* filename);
};

