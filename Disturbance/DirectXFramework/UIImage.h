#pragma once
#include "UIBase.h"

enum class eStateUI{off = 0, on};

class UIImage :
	public UIBase
{
	LPDIRECT3DTEXTURE9	mTextureUI;
	D3DXIMAGE_INFO		mImageInfo;
	LPD3DXSPRITE		mSprite;
	RECT				mDrawRect;

	string mStateUI[2];
	map<string, UIImage*> mChildSprite;
public:
	D3DXMATRIXA16 mMatWorld;
	UIImage(string texturePath, POINT pos, LONG width, LONG height);
	virtual ~UIImage();

	RECT GetDrawRect() const { return mDrawRect; }
	void SetDrawRect(RECT drawRect) { mDrawRect = drawRect; }

	void Render() override;
	void ShaderPart();
	
	void AddChild(string UIname, UIImage* child);

	UIImage* GetChildUI(string UIname)
	{	
		return mChildSprite.find(UIname)->second;
	}
	
	map<string,UIImage*> GetChildUI()
	{
		return mChildSprite;
	}

	void SetUIPath(string newTexturePath, eStateUI state)
	{
		mStateUI[static_cast<int>(state)] = newTexturePath;
	}
	
	void SetTexture(string newTexturePath)
	{
		D3DXIMAGE_INFO imageInfo;
		ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO));
		mTextureUI = gTextureManager->GetUITexture(newTexturePath, imageInfo);
	}

	void SetTexture(eStateUI state)
	{
		D3DXIMAGE_INFO imageInfo;
		ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO));
		mTextureUI = gTextureManager->GetUITexture(mStateUI[static_cast<int>(state)], imageInfo);
	}
	
};

