// OmokClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OmokClient.h"

#define WM_ASYNC  WM_USER+2

#define MAX_LOADSTRING 100

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
    LoadStringW(hInstance, IDC_OMOKCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOKCLIENT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOKCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OMOKCLIENT);
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
	   10, 10, 760, 800, nullptr, nullptr, hInstance, nullptr);

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
	//클라이언트 관련
	static WSADATA wsadata;
	static SOCKET s;
	static SOCKADDR_IN addr = { 0 };
	static char	buffer[100];
	static int mySocket;
	int		msgLen;
	
	//게임관련
	static std::vector<GamePacket> pos;
	static bool myturn = true;
	static bool Black;
	static bool observer = false;

    switch (message)
    {
	case WM_CREATE:
	{
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET)
		{
			//MessageBox(NULL, _T("socket failed"), _T("Error"), MB_OK);
			return 0;
		}
		else
		{
			//MessageBox(NULL, _T("socket success"), _T("Success"), MB_OK);
		}
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

		if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)
		{
			//MessageBox(NULL, _T("connect failed"), _T("Error"), MB_OK);
			return 0;
		}
		else
		{
			//MessageBox(NULL, _T("connect success"), _T("Success"), MB_OK);
		}

	}break;

	case WM_ASYNC:
	{
		switch (lParam)
		{
		case FD_READ:
			ServerInput(wParam, pos, mySocket, myturn, Black, observer);
			InvalidateRgn(hWnd, NULL, false);
			break;

		case FD_CLOSE:
			PostQuitMessage(0);
			break;
		}
	}break;
		
	case WM_LBUTTONDOWN:
	{
		if (!observer && myturn)
		{
			SentGame(s, lParam, mySocket, pos);
			InvalidateRgn(hWnd, NULL, false);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			system("cls");
			for (int i = 0; i < pos.size(); i++)
			{
				std::cout << pos[i].player << " : " << pos[i].index << std::endl;
			}

			if(myturn)
			{
				Rectangle(hdc, 0, 0, 800, 800);
			}
			else
			{
				HBRUSH myTurnBrush, oldBrush;
				myTurnBrush = CreateSolidBrush(RGB(200, 200, 200));
				oldBrush = (HBRUSH)SelectObject(hdc, myTurnBrush);
				Rectangle(hdc, 0, 0, 800, 800);
				SelectObject(hdc, oldBrush);
				DeleteObject(myTurnBrush);
				DeleteObject(oldBrush);
			}
			
			DrawOmokBoard(hdc);


			HBRUSH BlackBrush , WhiteBrush , oldBrush;
			BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
			WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));

			if (Black)
			{
				oldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
				for (int i = 0; i < pos.size(); i++)
				{
					if (pos[i].player == mySocket)
					{
						DrawCircle(hdc, { (pos[i].index % CELL_NUM) *CELL_PITCH + HALF_CELL_PITCH, (pos[i].index / CELL_NUM) * CELL_PITCH + HALF_CELL_PITCH },
							CELL_PITCH / 4);
					}
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(BlackBrush);
				DeleteObject(WhiteBrush);
				DeleteObject(oldBrush);
				
				for (int i = 0; i < pos.size(); i++)
				{
					if (pos[i].player != mySocket)
					{
						DrawCircle(hdc, { (pos[i].index % CELL_NUM) *CELL_PITCH + HALF_CELL_PITCH, (pos[i].index / CELL_NUM) * CELL_PITCH + HALF_CELL_PITCH },
							CELL_PITCH / 4);
					}
				}
			}

			else
			{
				
				for (int i = 0; i < pos.size(); i++)
				{
					if (pos[i].player == mySocket)
					{
						DrawCircle(hdc, { (pos[i].index % CELL_NUM) *CELL_PITCH + HALF_CELL_PITCH, (pos[i].index / CELL_NUM) * CELL_PITCH + HALF_CELL_PITCH },
							CELL_PITCH / 4);
					}
				}


				oldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
				for (int i = 0; i < pos.size(); i++)
				{
					if (pos[i].player != mySocket)
					{
						DrawCircle(hdc, { (pos[i].index % CELL_NUM) *CELL_PITCH + HALF_CELL_PITCH, (pos[i].index / CELL_NUM) * CELL_PITCH + HALF_CELL_PITCH },
							CELL_PITCH / 4);
					}
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(WhiteBrush);
				DeleteObject(BlackBrush);
				DeleteObject(oldBrush);
			}






			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		
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
