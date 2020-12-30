#pragma once
#include "Singleton.h"

#define gModelLoader ModelLoader::GetInstance()

struct staticXFile
{
	LPD3DXMESH			mpMesh;
	D3DMATERIAL9*		mpMaterials;
	LPDIRECT3DTEXTURE9*	mpTextures;
	DWORD				mNumMaterials;
};

class ModelLoader : public Singleton<ModelLoader>
{
private:
	map<string, staticXFile*> staticXFiles;

public:
	void LoadXfile(
		IN string					folder,
		IN string					fileName,
		OUT LPD3DXMESH&				mpMesh,
		OUT D3DMATERIAL9**			mpMaterials,
		OUT LPDIRECT3DTEXTURE9**	mpTextures,
		OUT DWORD&					mNumMaterials
	);
};

