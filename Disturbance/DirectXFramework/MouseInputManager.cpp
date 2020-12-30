#include "stdafx.h"
#include "MouseInputManager.h"

//extern HWND hWnd;
//
//BOOL MouseInputManager::DownLeftButton(POINT* const outPosition)
//{
//	bool bCurMouseLButton = GetKeyState(VK_LBUTTON) & 0x8000;
//
//	if (bCurMouseLButton)
//	{
//		GetCursorPos(outPosition);
//		ScreenToClient(hWnd, outPosition);
//		
//		mbLButtonDown = TRUE;
//		prevMousePosition = *outPosition;
//		
//		return true;
//	}
//
//	return false;
//}
//
//BOOL MouseInputManager::UpLeftButton(POINT* const outPosition)
//{
//	
//	static bool bPrevMouseLButton = false;
//	bool bCurMouseLButton = GetKeyState(VK_LBUTTON) & 0x8000;
//
//	if (bPrevMouseLButton == TRUE && bCurMouseLButton == FALSE)
//	{
//		GetCursorPos(outPosition);
//		ScreenToClient(hWnd, outPosition);
//		bPrevMouseLButton = bCurMouseLButton;
//		mbLButtonDown = FALSE;
//		return true; 
//	}
//
//	bPrevMouseLButton = bCurMouseLButton;
//	return false;
//}
//
//BOOL MouseInputManager::DownWheelButton(POINT * const outPosition)
//{
//	if (GetKeyState(VK_MBUTTON) & 0x8000)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//BOOL MouseInputManager::DragLeftButton(POINT * const outPosition)
//{
//	GetCursorPos(outPosition);
//	ScreenToClient(hWnd, outPosition);
//	
//	if (mbLButtonDown)
//	{
//		float fDeltaX = static_cast<float>(outPosition->x) - prevMousePosition.x;
//		float fDeltaY = static_cast<float>(outPosition->y) - prevMousePosition.y;
//		prevMousePosition = *outPosition;
//
//		outPosition->x = fDeltaX;
//		outPosition->y = fDeltaY;
//
//		return true;
//	}
//
//	return false;
//}
//
//BOOL MouseInputManager::DragRightButton(POINT * const outPosition)
//{
//	GetCursorPos(outPosition);
//	ScreenToClient(hWnd, outPosition);
//
//	static POINT pevMousePosition = *outPosition;
//
//	if (mMouseState == eEventName::MOUSE_L_DOWN)
//	{
//
//	}
//	
//}
//
//BOOL MouseInputManager::DownRightButton(POINT* const outPosition)
//{
//	static bool bPrevMouseRButton = false;
//	bool bCurMouseRButton = GetKeyState(VK_RBUTTON) & 0x8000;
//	
//	if (bCurMouseRButton)
//	{
//		GetCursorPos(outPosition);
//		ScreenToClient(hWnd, outPosition);
//		return true;
//	}
//	
//	return false;
//}
//
//BOOL MouseInputManager::UpRightButton(POINT* const outPosition)
//{
//	static bool bPrevMouseRButton = false;
//	bool bCurMouseRButton = GetKeyState(VK_RBUTTON) & 0x8000;
//
//	if (bPrevMouseRButton == TRUE && bCurMouseRButton == FALSE)
//	{
//		GetCursorPos(outPosition);
//		ScreenToClient(hWnd, outPosition);
//		bPrevMouseRButton = bCurMouseRButton;
//		return true;
//	}
//
//	bPrevMouseRButton = bCurMouseRButton;
//	return false;
//}
//
//BOOL MouseInputManager::MoveMouse(POINT* const outPosition)
//{
//	static POINT prevMousePosition;
//	POINT curMousePosition;
//
//	GetCursorPos(&curMousePosition);
//	ScreenToClient(hWnd, &curMousePosition);
//	
//	if (prevMousePosition.x != curMousePosition.x || prevMousePosition.y != curMousePosition.y)
//	{
//		*outPosition = curMousePosition;
//
//		return true;
//	}
//
//	return false;
//}
//void MouseInputManager::Update()
//{
//	POINT mousePosition;
//
//	if (gMouseInputManager->MoveMouse(&mousePosition))
//	{
//		gEventManager->EventOccurred(eEventName::MOUSE_MOVE, &mousePosition);
//	}
//	if (gMouseInputManager->DownLeftButton(&mousePosition))
//	{
//		gEventManager->EventOccurred(eEventName::MOUSE_L_DOWN, &mousePosition);
//	}
//	if (gMouseInputManager->DownRightButton(&mousePosition))
//	{
//		gEventManager->EventOccurred(eEventName::MOUSE_R_DOWN, &mousePosition);
//	}
//	if (gMouseInputManager->UpLeftButton(&mousePosition))
//	{
//		gEventManager->EventOccurred(eEventName::MOUSE_L_UP, &mousePosition);
//	}
//	if (gMouseInputManager->UpRightButton(&mousePosition))
//	{
//		gEventManager->EventOccurred(eEventName::MOUSE_R_UP, &mousePosition);
//	}
//
//}

void MouseInputManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!mbDisalble)
	{
		POINT curMousePosition;
		static POINT prevMosuePosition;

		curMousePosition.x = LOWORD(lParam);
		curMousePosition.y = HIWORD(lParam);

		switch (message)
		{
		case WM_LBUTTONDOWN:

			gEventManager->EventOccurred(eEventName::MOUSE_L_DOWN, &curMousePosition);
			prevMosuePosition = curMousePosition;
			mbLButtonDown = true;
			break;

		case WM_LBUTTONUP:

			gEventManager->EventOccurred(eEventName::MOUSE_L_UP, &curMousePosition);
			mbLButtonDown = false;
			break;

		case WM_RBUTTONUP:

			gEventManager->EventOccurred(eEventName::MOUSE_R_DOWN, &curMousePosition);
			prevMosuePosition = curMousePosition;
			mbRButtonDown = true;
			break;

		case WM_RBUTTONDOWN:

			gEventManager->EventOccurred(eEventName::MOUSE_R_UP, &curMousePosition);
			mbRButtonDown = false;
			break;

		case WM_MOUSEMOVE:
		{
			POINT mouseDeltaDistance;

			mouseDeltaDistance.x = curMousePosition.x - prevMosuePosition.x;
			mouseDeltaDistance.y = curMousePosition.y - prevMosuePosition.y;

			gEventManager->EventOccurred(eEventName::MOUSE_MOVE, &curMousePosition);

			if (mbLButtonDown)
			{
				gEventManager->EventOccurred(eEventName::MOUSE_L_DRAG, &mouseDeltaDistance);
				prevMosuePosition = curMousePosition;
			}
			else if (mbRButtonDown)
			{
				gEventManager->EventOccurred(eEventName::MOUSE_R_DRAG, &mouseDeltaDistance);
				prevMosuePosition = curMousePosition;
			}

			break;
		}
		case WM_MBUTTONDOWN:
			gEventManager->EventOccurred(eEventName::MOUSE_WHEEL_DOWN, &curMousePosition);
			break;

		case WM_MOUSEWHEEL:
			float wheelDeltaDistance(GET_WHEEL_DELTA_WPARAM(wParam) / 30.0f);
			gEventManager->EventOccurred(eEventName::MOUSE_WHEEL_SCROLL, &wheelDeltaDistance);
			break;
		}
	}
}

void MouseInputManager::Disabled()
{
	mbDisalble = true;
}

void MouseInputManager::Abled()
{
	mbDisalble = false;
}


