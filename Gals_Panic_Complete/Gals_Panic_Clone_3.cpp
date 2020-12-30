// Gals_Panic_Clone_3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Gals_Panic_Clone_3.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
Map WorldMap;
Character MainChar;

deque<Monster*> MonQue;

Menu MainMenu;
//Monster MainMon;
HDC hMemDC;
HBITMAP hOldBitmap, Bitmap;
int bx, by;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CALLBACK	KeyProc(HWND, UINT, UINT, DWORD);

HBITMAP hBackImage;
BITMAP bitBack;
RECT wndView;


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
    LoadStringW(hInstance, IDC_GALS_PANIC_CLONE_3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALS_PANIC_CLONE_3));

    MSG msg;
	hInst = hInstance;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALS_PANIC_CLONE_3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALS_PANIC_CLONE_3);
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
      10, 10, 818, 860, nullptr, nullptr, hInstance, nullptr);

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
		
		for (int i = 0; i < DEFALTMONSTER; i++)
		{
			Monster* temp = new Monster;
			MonQue.push_front(temp);
		}
		
		SetTimer(hWnd, 1, 50, false);
		SetTimer(hWnd, 2, 50, (TIMERPROC)KeyProc);
		//hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5_2));
		CreateBitmap(hBackImage, bitBack);
		GetClientRect(hWnd, &wndView);
		}
		break;

	case WM_SIZE:
	{
		GetClientRect(hWnd, &wndView);
	}
	break;
	case WM_TIMER:
		{
		switch (wParam)
		{
		case 1:
			if (!MainMenu.IsEndState())
			{

				MainChar.IsOnTheLine();
			
				if(MonQue.size() == 0)
					MainMenu.SetEndState(true);

				for (int i = 0; i < MonQue.size(); i++)
				{
					if (MonQue[i]->Update())
						MainMenu.SetEndState(true);
				}


				for (deque<Monster*>::iterator it = MonQue.begin(); it != MonQue.end(); )
				{
					if ((*it)->delTriger)
					{
						delete (*it);
						it = MonQue.erase(it);
					}
					else
					{
						it++;
					}
				}

				InvalidateRect(hWnd, NULL, false);
			}
			break;
		}
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
	

	case WM_LBUTTONDOWN:
	{
		if(MainMenu.IsStartState())
			MainMenu.PutStartClick(lParam);
		if (MainMenu.IsEndState())
		{
			if (MainMenu.PutEndClick(lParam))
			{
				MainChar.Restart();

				for (deque<Monster*>::iterator it = MonQue.begin(); it != MonQue.end(); it++)
				{
					delete (*it);
				}
				MonQue.clear();
				for (int i = 0; i < DEFALTMONSTER; i++)
				{
					Monster* temp = new Monster;
					MonQue.push_front(temp);
				}
			}		
		}	
	}
	break;


    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC hMemDC, hMemDC2, hMemDC3;
			HBITMAP HMemBit, HMemBit2, oldBit, oldBit2;

			hMemDC = CreateCompatibleDC(hdc);
			hMemDC3 = CreateCompatibleDC(hdc);
			HMemBit = CreateCompatibleBitmap(hdc, wndView.right, wndView.bottom);
			SelectObject(hMemDC3, HMemBit);
			
			oldBit = (HBITMAP)SelectObject(hMemDC, hBackImage);
			
			hMemDC2 = CreateCompatibleDC(hdc);
			HMemBit2 = CreateCompatibleBitmap(hMemDC, wndView.right, wndView.bottom);
			oldBit2 = (HBITMAP)SelectObject(hMemDC2, HMemBit2);

			bx = bitBack.bmWidth;
			by = bitBack.bmHeight;

			WorldMap.Draw(hMemDC2);
			MainChar.Draw(hMemDC2);
			for (int i = 0; i < MonQue.size(); i++)
			{
				MonQue[i]->Draw(hMemDC2);
			}


			if(MainMenu.IsStartState())
				MainMenu.StartDraw(hMemDC2);

			if (MainMenu.IsEndState())
				MainMenu.EndDraw(hMemDC2);

			BitBlt(hMemDC3, 0, 0, wndView.right, wndView.bottom, hMemDC, 0, 0, SRCCOPY);

			TransparentBlt(hMemDC3, 0, 0, wndView.right, wndView.bottom, hMemDC2,
				0, 0, wndView.right, wndView.bottom, RGB(255, 0, 255));

			BitBlt(hdc, 0, 0, wndView.right, wndView.bottom, hMemDC3, 0, 0, SRCCOPY);


			SelectObject(hMemDC, oldBit);
			SelectObject(hMemDC2, oldBit2);
			DeleteObject(HMemBit);
			DeleteObject(HMemBit2);
			DeleteObject(oldBit);
			DeleteObject(oldBit2);
			DeleteDC(hMemDC);
			DeleteDC(hMemDC2);
			DeleteDC(hMemDC3);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		for (int i = 0; i < MonQue.size(); i++)
		{
			delete MonQue[i];
		}
		DeleteObject(Bitmap);
		DeleteBitmap(hBackImage);
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

void CALLBACK KeyProc(HWND hDlg, UINT message, UINT idEvent, DWORD dwTime)
{
	if ((MainMenu.IsEndState()) || (MainMenu.IsStartState()))
		return;

	if (GetKeyState(VK_UP) & 0x8000)
	{
		MainChar.PlusPos({ -MOVESPEED, 0 });
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		MainChar.PlusPos({ 0, MOVESPEED });
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		MainChar.PlusPos({ MOVESPEED, 0 });
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		MainChar.PlusPos({ 0, -MOVESPEED });
	}
	MainChar.Escape();
	MainChar.Update();
	return;
}





void CreateBitmap(HBITMAP& hBackImage, BITMAP& bitBack)
{
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("image/webear.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage, sizeof(BITMAP), &bitBack);
}

void DrawBitmap(HWND hWnd, HDC hdc, HBITMAP& hBackImage, BITMAP& bitBack)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
}

void DeleteBitmap(HBITMAP& hBackImage)
{
	DeleteObject(hBackImage);
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC, hMemDC2, hMemDC3;
	HBITMAP HMemBit, HMemBit2, oldBit, oldBit2;

	hMemDC = CreateCompatibleDC(hdc);
	hMemDC3 = CreateCompatibleDC(hdc);
	HMemBit = CreateCompatibleBitmap(hdc, wndView.right, wndView.bottom);
	SelectObject(hMemDC3, HMemBit);
	
	oldBit = (HBITMAP)SelectObject(hMemDC, hBackImage);
	
	hMemDC2 = CreateCompatibleDC(hdc);
	HMemBit2 = CreateCompatibleBitmap(hMemDC, wndView.right, wndView.bottom);
	oldBit2 = (HBITMAP)SelectObject(hMemDC2, HMemBit2);

	bx = bitBack.bmWidth;
	by = bitBack.bmHeight;

	
	WorldMap.Draw(hMemDC2);
	MainChar.Draw(hMemDC2);

	BitBlt(hMemDC3, 0, 0, wndView.right, wndView.bottom, hMemDC, 0, 0, SRCCOPY);

	TransparentBlt(hMemDC3, 0, 0, wndView.right, wndView.bottom, hMemDC2,
		0, 0, wndView.right, wndView.bottom, RGB(255, 0, 255));

	BitBlt(hdc, 0, 0, wndView.right, wndView.bottom, hMemDC3, 0, 0, SRCCOPY);


	SelectObject(hMemDC, oldBit);
	SelectObject(hMemDC2, oldBit2);
	DeleteObject(HMemBit);
	DeleteObject(HMemBit2);
	DeleteObject(oldBit);
	DeleteObject(oldBit2);
	DeleteDC(hMemDC);
	DeleteDC(hMemDC2);
	DeleteDC(hMemDC3);
}