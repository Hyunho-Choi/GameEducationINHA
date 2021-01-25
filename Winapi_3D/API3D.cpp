// API3D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "API3D.h"
#include "cMainGame.h"

#define MAX_LOADSTRING 100


HWND g_hWnd;
cMainGame * g_pMainGame;
#define TIMER_ID 123




// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
    LoadStringW(hInstance, IDC_API3D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_API3D));


	// 입력코드
	g_pMainGame = new cMainGame;
	g_pMainGame->Setup();
	SetTimer(g_hWnd, TIMER_ID, 10, NULL);
	




	
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
			
		}
	}

	KillTimer(g_hWnd, TIMER_ID);
	delete g_pMainGame;
	
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_API3D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_API3D);
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
      CW_USEDEFAULT, 0, WIN_WIDE, WIN_HIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
	
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
	
	if(g_pMainGame)
	{
		g_pMainGame->WndProc(hWnd, message, wParam, lParam);
	}
	

	
	static std::vector<Object*> objCon;
	static POINT lastMousePos;
	static float CameraPosX = 0;
	static float CameraPosY = 0;

	static Object Camera(0, 5, -10);
	
	//static cVector3 Eye(CameraPosX, CameraPosY, -10);
	static cVector3 Target(0, 0, 0);

	
    switch (message)
    {
	case WM_CREATE:
		{
			//SetTimer(hWnd, 0, 100, NULL);
			//objCon.push_back(new Object(CUBE));
			//objCon.push_back(new Object(2.f,2.f,2.f,CUBE));
			//objCon.push_back(new Object(GRID));
		}
		break;
	case WM_TIMER:
		{
			if(g_pMainGame)
			{
				g_pMainGame->Update();
				InvalidateRect(g_hWnd, nullptr, false);
			}
			
		}
		break;
		/*
	case WM_MOUSEMOVE:
		{
			switch (wParam)
			{
			case MK_LBUTTON:
				for (auto it = objCon.begin(); it != objCon.end(); ++it)
				{
					(*it)->Rotate((lastMousePos.x - LOWORD(lParam)) / 5.f, enumY);
					(*it)->Rotate((HIWORD(lParam) - lastMousePos.y) / 5.f, enumX);
				}
				InvalidateRect(hWnd, nullptr, true);
				break;

			case MK_LBUTTON | MK_SHIFT:
				for (auto it = objCon.begin(); it != objCon.end(); ++it)
				{
					(*it)->Move((LOWORD(lParam) - lastMousePos.x) / 100.f, enumX);
					(*it)->Move((lastMousePos.y - HIWORD(lParam)) / 100.f, enumY);
				}
				InvalidateRect(hWnd, nullptr, true);
				break;
			}
			
			lastMousePos = { LOWORD(lParam), HIWORD(lParam) };
		}
		break;
    	
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_UP:
				CameraPosY += 0.3;
				break;
			case VK_DOWN:
				CameraPosY -= 0.3;
				break;
			case VK_RIGHT:
				CameraPosX += 0.3;
				break;
			case VK_LEFT:
				CameraPosX -= 0.3;
				break;
		
			}
		}
    	break;
    	*/
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

    		/*
			int start = GetTickCount();
			for (auto it = objCon.begin(); it != objCon.end(); ++it)
			{
				(*it)->Render(hdc, Camera.GetPosition(), Target);
			}
			std::cout << GetTickCount() - start << std::endl;
			*/
    		
			if (g_pMainGame)
				g_pMainGame->Render(hdc);

    		
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		/*
		for(auto it = objCon.begin(); it != objCon.end(); ++it)
		{
			delete *it;
		}
		KillTimer(hWnd, 0);
		*/

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
