//VIEW - PROPERTY MANAGER
//Debug | Win32 --> Microsoft.Cpp.Win32.user
//Include Directories - Make sure C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include has been added.
//Library Directories - Make sure C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86

//Linker Input - Make sure d3d9.lib and d3dx9.lib are there.

//Windows.h include necessary functions for Windows API
#include <Windows.h>

#include <Windowsx.h>  //not in the documentation but needed for GET_X_LPARAM

#include "Game.h"

//Prototypes for the GenerateWindow function, otherwise WinMain cannot call GenerateWindow.
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height, HWND& hWnd);
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height, HWND& hWnd);

//Another prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool MyHandleMouseEvent(MSG *msg);
bool MyHandleKeyEvent(MSG *msg);

Game *game;

//WinMain function, set up window and message loop.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

	if(GenerateWindow(hInstance, nCmdShow, "SpriteUpdateWithTimer", "Updating Sprite with Timers Sample", 1600, 900, hWnd))
	{
		MSG msg;
		game = new Game();

		if(game->Initialize(hWnd))
		{
			while(true)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD))
				{
					bool fHandled = false;
					if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
						fHandled = MyHandleMouseEvent(&msg);
					else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
						fHandled = MyHandleKeyEvent(&msg);
					else if (WM_QUIT == msg.message)
						break;

					if (!fHandled)
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					game->Run();
				}
			}

			delete game;
//			exit(0); // added because visual c++ didn't exit the program
			return msg.wParam;
		}
	}
//	exit(0); // added because visual c++ didn't exit the program
	return 0;
}

bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int width, int height, HWND& hWnd)
{
	return GenerateWindow(hInstance, nCmdShow, className, windowTitle, (GetSystemMetrics(SM_CXSCREEN) - width)/2, 
		(GetSystemMetrics(SM_CYSCREEN) - height)/2, width, height, hWnd);
}

//Generic GenerateWindow function.
bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height, HWND& hWnd)
{
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if(!RegisterClassEx(&wcex))
	{
		return false;
	}

	//Now we are using Direct3D, we need to make sure the window will not be adjusted by the user.
	hWnd = CreateWindowEx(NULL, className, windowTitle, WS_POPUP, x, y, width, height, NULL, NULL, hInstance, NULL);
//	hWnd = CreateWindowEx(NULL, className, windowTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width, height, NULL, NULL, hInstance, NULL);
//	hWnd = CreateWindowEx(NULL, className,"", WS_VISIBLE, x, y, width, height, NULL, NULL, hInstance, NULL);
//	SetWindowLong(hWnd, GWL_STYLE, 0);
	ShowWindow(hWnd, nCmdShow);

	return true;
}

//WindowProc - Handles input sent to the window.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
bool MyHandleMouseEvent(MSG *msg)
{
	int x, y;
	char mybuff[1025];
	if (msg->message == WM_LBUTTONDOWN)
	{
		game->udp_debug_print("mouse event");
		x = GET_X_LPARAM(msg->lParam);
		y = GET_Y_LPARAM(msg->lParam);
		sprintf_s(mybuff, sizeof(mybuff), "mouse at x=%d and y=%d\n", x,y);
		game->udp_debug_print(mybuff);
		sprintf_s(mybuff, sizeof(mybuff), "mouse at x=%d and y=%d\n", x, y);
		game->udp_debug_print_debug(mybuff);
	}
	return(false);
}

bool MyHandleKeyEvent(MSG *msg)
{
	return(false);
}