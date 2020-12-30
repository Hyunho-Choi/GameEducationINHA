#pragma once
#include "EventManager.h"
#include "Singleton.h"

#define gMouseInputManager MouseInputManager::GetInstance()

class MouseInputManager : public Singleton<MouseInputManager>
{
public:
	void Disabled();
	void Abled();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool mbDisalble = false;
	bool mbLButtonDown;
	bool mbRButtonDown;

};
