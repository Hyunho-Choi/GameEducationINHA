#include "stdafx.h"
#include "FlipAnimation.h"

HBITMAP FlipAnimation::hAniImage = (HBITMAP)LoadImage(NULL, TEXT("image/zero_run.bmp"),
	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

FlipAnimation::FlipAnimation()
{
	GetObject(hAniImage, sizeof(BITMAP), &bitAni);
}


FlipAnimation::~FlipAnimation()
{
}

bool FlipAnimation::GetDelTriger()
{
	return delTriger;
}


void FlipAnimation::DrawBitmap(HDC hdc)
{
	if (!delTriger)
	{
		HBITMAP hOldBitmap;
		hOldBitmap = (HBITMAP)SelectObject(hdc, hAniImage);

		TransparentBlt(hdc, 200, 100, 10, 10, hdc,
			cur_Frame * 10, 0, 10, 10, RGB(255, 0, 255));
		BitBlt(hdc, 200, 100, 10, 10, )

		SelectObject(hdc, hOldBitmap);
	}
}

void FlipAnimation::DeleteBitmap()
{

}

void FlipAnimation::UpdateFrame()
{
	if (cur_Frame > 7)
		delTriger = true;
	else
	{
		//cur_Frame++;
	}
		
}
