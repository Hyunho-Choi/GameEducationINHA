// BrawlStartsClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BrawlStartsClient.h"

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int gameMode = GAME_MODE;
UIManager Menu;
UserController UserCon;
LocalClient LoClient;
LocalServer LoServer;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BRAWLSTARTSCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRAWLSTARTSCLIENT));

    MSG msg;

    // Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Key Down Sequence
			switch (gameMode)
			{
			case LOBBY:
				break;
			case INGAME:
				UserCon.PushKeyboardInput();
				break;
			case ONLINELOBBY:
				break;
			}
		}
	}
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRAWLSTARTSCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BRAWLSTARTSCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WIN_1280, WIN_720, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			SetTimer(hWnd, 0, 20, NULL);
			SetTimer(hWnd, 1, 60, NULL);
		}
		break;

	case WM_TIMER:
		{
		switch (wParam)
		{
		case 0:
			switch (gameMode)
			{
			case LOBBY:
				break;
			case INGAME:
				LoServer.ReceiveControllerBuffer(UserCon.GetBuffer());
				LoServer.SendUserDateToClient(0, &LoClient.UserBuffer[0]);
				LoServer.SendUserDateToClient(1, &LoClient.UserBuffer[1]);
				LoServer.UpdateBullet();
				LoClient.Update();
				;
				InvalidateRect(hWnd, NULL, false);
				break;
			case ONLINELOBBY:
				break;
			}
			break;
		case 1:
			switch (gameMode)
			{
			case LOBBY:
				InvalidateRect(hWnd, NULL, false);
				break;
			}
			break;
		}

			
		}
		break;
	case WM_GETMINMAXINFO: //화면 크기 고정
		{
			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = WIN_1280;
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = WIN_720;

			((MINMAXINFO*)lParam)->ptMinTrackSize.x = WIN_1280;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = WIN_720;
		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
		/*
	case WM_KEYDOWN:
		{
			switch (gameMode)
			{
			case LOBBY:
				break;
			case INGAME:
				UserCon.PushKeyboardInput();
				break;
			case ONLINELOBBY:
				break;
			}
		}
		break;
		*/

	case WM_MOUSEMOVE:
		{
			switch (gameMode)
			{
			case LOBBY:
				break;
			case INGAME:
				UserCon.PushMouseInput(lParam);
				break;
			case ONLINELOBBY:
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			switch (gameMode)
			{
			case LOBBY:
				if (!Menu.InputMouseClick(lParam))
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DISABLE), hWnd, About);
				}
				
				break;
			case INGAME:
				UserCon.PushClickInput(lParam);
				std::cout << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;

				break;
			case ONLINELOBBY:
				break;
			}
		
			RECT temp;
			GetClientRect(hWnd, &temp);
			printf("X : %d / y : %d\n", temp.right, temp.bottom);
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
	
			int start = GetTickCount();
			switch (gameMode)
			{
			case LOBBY:
				Menu.Draw(hdc);
				break;
			case INGAME:
				LoClient.Draw(hdc);
				break;
			case ONLINELOBBY:
				break;
			}
			int end = GetTickCount();
			std::cout << end - start << std::endl;

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
		KillTimer(hWnd, 0);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

