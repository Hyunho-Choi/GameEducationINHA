// OmokGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OmokGame.h"



#define MAX_LOADSTRING 100
#define WM_ASYNC  WM_USER+2
using namespace std;

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
    LoadStringW(hInstance, IDC_OMOKGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOKGAME));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOKGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OMOKGAME);
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
      10, 10, 300,300, nullptr, nullptr, hInstance, nullptr);

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
	//서버관련
	static WSADATA wsadata;
	static SOCKET s;
	
	static vector<SOCKET> clientSocket;
	static SOCKET clientSocket2[4] = {NULL, NULL, NULL, NULL};

	static SOCKADDR_IN addr = { 0 };
	static SOCKADDR_IN c_addr;
	static int ClientCnt = 0;
	int size;

	//게임관련
	static char buffer[100];
	static vector<GamePacket> pos;
	static SOCKET nowPlayer = 0;

    switch (message)
    {
	case WM_CREATE:
	{
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (bind(s, (LPSOCKADDR)& addr, sizeof(addr)))
		{
			//MessageBox(NULL, _T("binding failed"), _T("Error"), MB_OK);
			return 0;
		}
		else
		{
			//MessageBox(NULL, _T("binding success"), _T("Success"), MB_OK);
		}

		WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_ACCEPT);

		if (listen(s, 5) == SOCKET_ERROR)
		{
			//MessageBox(NULL, _T("listen failed"), _T("Error"), MB_OK);
			return 0;
		}
		else
		{
			//MessageBox(NULL, _T("listen success"), _T("Success"), MB_OK);
		}
	}break;

	case WM_ASYNC:
	{
		switch (lParam)
		{
		case FD_ACCEPT:
			size = sizeof(c_addr);
			if (ClientCnt == 0) //클라이언트 0 흑돌
			{
				clientSocket2[0] = accept(s, (LPSOCKADDR)&c_addr, &size);
				WSAAsyncSelect(clientSocket2[0], hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
				nowPlayer = clientSocket2[0];
				cout << "Connect Client 00 " << clientSocket2[0] << endl;
				ReturnSocketNum(clientSocket2[0], 1);
			}
			else if (ClientCnt == 1) //클라이언트 1 백돌
			{
				clientSocket2[1] = accept(s, (LPSOCKADDR)&c_addr, &size);
				WSAAsyncSelect(clientSocket2[1], hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
				cout << "Connect Client 01 " << clientSocket2[1] << endl;
				ReturnSocketNum(clientSocket2[1], 0);
			}
			else if (ClientCnt == 2) //클라이언트 2 관전
			{
				clientSocket2[2] = accept(s, (LPSOCKADDR)&c_addr, &size);
				WSAAsyncSelect(clientSocket2[2], hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
				cout << "Connect Client 02 " << clientSocket2[2] << endl;
				ReturnSocketNum(clientSocket2[2], 2);
				SendNowGameToObserver(clientSocket2[2], pos);
			}
			else if (ClientCnt == 3) //클라이언트 3 관전
			{
				clientSocket2[3] = accept(s, (LPSOCKADDR)&c_addr, &size);
				WSAAsyncSelect(clientSocket2[3], hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
				cout << "Connect Client 03 " << clientSocket2[3] << endl;
				ReturnSocketNum(clientSocket2[3], 2);
				SendNowGameToObserver(clientSocket2[3], pos);
			}
			ClientCnt++;
			break;
		case FD_READ:
			if (nowPlayer == wParam)
			{
				ClientInput(wParam, nowPlayer, pos, clientSocket2);

				if (nowPlayer == clientSocket2[0])
					nowPlayer = clientSocket2[1];
				else if (nowPlayer == clientSocket2[1])
					nowPlayer = clientSocket2[0];
				
				InvalidateRgn(hWnd, NULL, false);
			}
			break;
		case FD_CLOSE:
			cout << "Close Client : " << wParam << endl ;
			ClientCnt--;
			if((wParam == clientSocket2[0]) || (wParam == clientSocket2[1]))
			{
				//리셋
				SendResetPacket(clientSocket2);

			}
			break;
		}
	}break; 

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
			//system("cls");
			if(pos.size() != 0)
				cout << pos[pos.size()-1].player << " : " << pos[pos.size() - 1].index << endl;
			
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
