#include "stdafx.h"
#include "SceneCenter.h"
#include "Scene.h"
#include "MouseInputManager.h"

extern CRITICAL_SECTION CS;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool GenerateWindow(HINSTANCE hIns, int nCmdShow, LPWSTR className, LPWSTR windowTitle, UINT windowWidth = 1600,
                    UINT windowHeight = 900);
HWND hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hIns,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
	//gJSON->LoadJSON("jsonTest.json");
	//gJSON->WriteJSON("jsonTest2.json");
	if (GenerateWindow(hIns, nCmdShow, L"Win32Window", L"DirectXFramework"))
	{
		InitializeCriticalSection(&CS);
		MSG msg;
		SceneCenter sceneCenter;
		while (true)
		{
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				sceneCenter.InputProcess();
				sceneCenter.Update();
				sceneCenter.Render();
			}
		}
		DeleteCriticalSection(&CS);
		return msg.wParam;
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	gMouseInputManager->WndProc(hWnd, message, wParam, lParam);
	
	switch (message)
	{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		default:
			{
				DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool GenerateWindow(HINSTANCE hIns, int nCmdShow, LPWSTR className, LPWSTR windowTitle, UINT windowWidth,
                    UINT windowHeight)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW; // HREDARW = 윈도우 세로 변경 시 새로 그림 VREDRAW = 가로
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hIns;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	if (RegisterClassEx(&wcex) == false)
	{
		return false;
	}

	hWnd = CreateWindowEx(NULL, className, windowTitle, WS_EX_TOPMOST | WS_POPUP,
	                      (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2,
	                      (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2,
	                      windowWidth, windowHeight, nullptr, nullptr, hIns, nullptr);
	ShowWindow(hWnd, nCmdShow);
	return true;
}
