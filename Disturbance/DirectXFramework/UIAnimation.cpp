#include "stdafx.h"
#include "UIImage.h"
#include "UIAnimation.h"

UIAnimation::UIAnimation(string texturePath, POINT pos, LONG width, LONG height, LONG srcNumRaw) 
	: UIImage(texturePath, pos, width, height)
{
	mNumRaw = srcNumRaw;
	mOriRect = GetDrawRect();
	mLenOneRaw = GetDrawRect().bottom / mNumRaw;
	mCntTime = 0.f;
	mTimePerFrame = 1.f / mNumRaw;

	SetRect(&mAnimRect, 0, 0, mOriRect.right, mLenOneRaw);
}

UIAnimation::~UIAnimation()
{

}

void UIAnimation::Update()
{
	mCntTime += gDeltaTime;
	if (mCntTime > mTimePerFrame)
	{
		if (mAnimRect.bottom >= mOriRect.bottom)
		{
			SetRect(&mAnimRect, 0, 0, mOriRect.right, mLenOneRaw);
		}
		else 
		{
			mAnimRect.top += mLenOneRaw;
			mAnimRect.bottom += mLenOneRaw;
		}
		SetDrawRect(mAnimRect);
		mCntTime = 0.f;
	}
}

void UIAnimation::Render()
{
	UIImage::Render();
}

