#pragma once
#include "BaseObject.h"
#include "BaseObserver.h"

#define HORIZONTAL_CELL_NUM 64
#define VERTICAL_CELL_NUM 36

class UIBase :
	public BaseObject, public BaseObserver
{
protected:
	/*D3DXVECTOR3 mPos;
	float		mWidth;
	float		mHeight;*/
	POINT		mPos;
	LONG		mWidth;
	LONG		mHeight;
	bool		mbVisible;
public:
	/*D3DXVECTOR3 GetPos() const
	{
		return mPos;
	}

	void SetPos(const D3DXVECTOR3& m_pos)
	{
		mPos = m_pos;
	}


	float GetWidth() const
	{
		return mWidth;
	}

	void SetWidth(float m_width)
	{
		mWidth = m_width;
	}


	float GetHeight() const
	{
		return mHeight;
	}

	void SetHeight(float m_height)
	{
		mHeight = m_height;
	}*/


	POINT GetPos() { return mPos; }
	void SetPos(POINT& pos) { mPos = pos; }

	LONG GetWidth() { return mWidth; }
	void SetWidth(LONG& width) { mWidth = width; }

	LONG GetHeight() { return mHeight; }
	void SetHeight(LONG& height) { mHeight = height; }
	
	bool IsVisible() const
	{
		return mbVisible;
	}

	void SetVisible(bool mb_visible)
	{
		mbVisible = mb_visible;
	}


	RECT GetRectInViewPort();
	
	UIBase();
	virtual ~UIBase();

	function<bool(eEventName, void*, UIBase *)> EventProcess;
	bool Update(eEventName eventName, void* parameter) override;
};
