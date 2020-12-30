#include "stdafx.h"
#include "TextureManager.h"


LPDIRECT3DTEXTURE9 TextureManager::GetTexture(char * szFullPath)
{
	if (mTextureContainer.find(szFullPath) == mTextureContainer.end())
	{
		D3DXCreateTextureFromFileA(gD3Device, szFullPath, &mTextureContainer[szFullPath]);
	}
	return mTextureContainer[szFullPath];
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(std::string & sFullPath)
{
	return GetTexture((char*)sFullPath.c_str());
}

LPDIRECT3DTEXTURE9 TextureManager::GetUITexture(char* szFullPath, D3DXIMAGE_INFO& imageInfo)
{
	if (mUITextureContainer.find(szFullPath) == mUITextureContainer.end())
	{
		D3DXCreateTextureFromFileExA(gD3Device, szFullPath,
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, 
			&imageInfo, nullptr, &mUITextureContainer[szFullPath]);
	}
	else
	{
		D3DXGetImageInfoFromFileA(szFullPath, &imageInfo);
	}
	return mUITextureContainer[szFullPath];
}

LPDIRECT3DTEXTURE9 TextureManager::GetUITexture(std::string& sFullPath, D3DXIMAGE_INFO& imageInfo)
{
	return GetUITexture((char*)sFullPath.c_str(), imageInfo);
}

void TextureManager::Destroy()
{
	for each(auto it in mTextureContainer)
	{
		SAFE_RELEASE(it.second);
	}
	mTextureContainer.clear();
}
