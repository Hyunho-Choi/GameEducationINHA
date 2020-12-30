#pragma once

class UIAnimation :
	public UIImage
{
private:
	RECT mOriRect;
	RECT mAnimRect;
	LONG mNumRaw;
	LONG mLenOneRaw;
	float mCntTime;
	float mTimePerFrame;


public:
	UIAnimation(string texturePath, POINT pos, LONG width, LONG height, LONG srcNumRaw);
	virtual ~UIAnimation();

	void Update() override;
	void Render() override;
};

