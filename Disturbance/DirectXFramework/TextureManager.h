#pragma once
#include "Singleton.h"
#define gTextureManager TextureManager::GetInstance()

class TextureManager : public Singleton<TextureManager>
{
private:
	map<string, LPDIRECT3DTEXTURE9> mTextureContainer;
	map<string, LPDIRECT3DTEXTURE9> mUITextureContainer;
public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string & sFullPath);

	LPDIRECT3DTEXTURE9 GetUITexture(char* szFullPath, OUT D3DXIMAGE_INFO& imageInfo);
	LPDIRECT3DTEXTURE9 GetUITexture(std::string & sFullPath, OUT D3DXIMAGE_INFO& imageInfo);
	void Destroy();
	
	
};

