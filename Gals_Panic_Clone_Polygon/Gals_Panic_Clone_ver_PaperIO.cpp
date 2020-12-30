// Gals_Panic_Clone_ver_PaperIO.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Gals_Panic_Clone_ver_PaperIO.h"

#define MAX_LOADSTRING 100

void CreateBitmap(HBITMAP& hBackImage, BITMAP& bitBack);
void DeleteBitmap(HBITMAP& hBackImage);
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int bx, by;
static Character ManiChar;
HBITMAP hBackImage;
BITMAP bitBack;
RECT wndView;

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
    LoadStringW(hInstance, IDC_GALS_PANIC_CLONE_VER_PAPERIO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALS_PANIC_CLONE_VER_PAPERIO));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALS_PANIC_CLONE_VER_PAPERIO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALS_PANIC_CLONE_VER_PAPERIO);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
			SetTimer(hWnd, 1, 50, NULL); //draw 타이머
			SetTimer(hWnd, 2, 50, NULL); //move 타이머
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
				InvalidateRect(hWnd, NULL, false);
				break;

			case 2:
				ManiChar.Update();
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
	case WM_KEYDOWN:
		{
			ManiChar.Controllor();
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

			DrawBitmapDoubleBuffering(hWnd, hdc);

			//ManiChar.Draw(hdc);



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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



void CreateBitmap(HBITMAP& hBackImage, BITMAP& bitBack)
{
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("image.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage, sizeof(BITMAP), &bitBack);
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

	ManiChar.Draw(hMemDC2);

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