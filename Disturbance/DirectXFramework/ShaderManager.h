#pragma once
#include "Base3DObject.h"
#include "Singleton.h"

#define gShader ShaderManager::GetInstance()


class ShaderManager : public Singleton<ShaderManager>
{
private:
	map<string, LPD3DXEFFECT>		mShaders;
	LPDIRECT3DTEXTURE9				mTexA1, mTexFloor;
	LPDIRECT3DTEXTURE9				mTexRound, mTexVoronoi, mTexCCTV;

public:
	void LoadAllShader();
	LPD3DXEFFECT LoadShader(const char* filename);
	LPDIRECT3DTEXTURE9 LoadTexture(const char * filename);
	
	void RenderWithToonShader(function<void()> FunctionPtr);
	void RenderWithFireShader(function<void()> FunctionPtr);
	void RenderWithWallShader(function<void()> FunctionPtr);
	void RenderWithItemShader(function<void()> FunctionPtr, eShaderPath eShaderPath);
	void RenderWithFlashShader(function<void()> FunctionPtr);
	void RenderWithFloorShader(function<void()> FunctionPtr);
	void RenderWithPortalShader(function<void()> FunctionPtr);
	void RenderWithCCTVShader(function<void()> FunctionPtr);
};

