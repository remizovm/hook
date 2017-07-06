/*
TODO:
- init d3d9
- init d3d10
- init d3d11
- init d3d12
*/

#include "stdafx.h"
#include "D3D9.h"
#include "D3D11.h"
void HandleLastError(void);
bool useDX9 = FALSE;
bool useDX11 = FALSE;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CREATE:
		{
			HWND Button1 = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"это рисует на DX9",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				10,         // x position 
				10,         // y position 
				300,        // Button width
				60,        // Button height
				hWnd,     // Parent window
				(HMENU)9,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.
			HWND Button2 = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"это рисует на DX11",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				320,         // x position 
				10,         // y position 
				300,        // Button width
				60,        // Button height
				hWnd,     // Parent window
				(HMENU)11,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.
			return 0;
		}
		case WM_COMMAND:
			if (LOWORD(wParam) == 9)
			{
				useDX9 = !useDX9;
			}
			else if (LOWORD(wParam) == 11)
			{
				useDX11 = !useDX11;
			}
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void HandleLastError(void)
{
	auto dw = GetLastError();
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("failed with error %d: %s"), dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wc = { 0 };
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";
	if ( !RegisterClassEx(&wc) ) {
		HandleLastError();
		return 1;
	}
	auto hWnd = CreateWindowEx(NULL, L"WindowClass", L"Test", 
							   WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, 
							   NULL, hInstance, NULL);
	if ( hWnd == NULL ) {
		UnregisterClass(L"WindowClass", hInstance);
		HandleLastError();
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	HWND Button3 = CreateWindow(
		L"Button",  // Predefined class; Unicode assumed 
		L"",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		80,         // y position 
		400,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	HWND Button4 = CreateWindow(
		L"Button",  // Predefined class; Unicode assumed 
		L"",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		200,         // y position 
		400,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	auto d3d9 = make_unique<D3D9>(Button3);
	d3d9->Init();
	auto d3d11 = make_unique<D3D11>(Button4);
	d3d11->Init();
	MSG msg;
	while ( 1 ) {
		while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ( msg.message == WM_QUIT ) {
			break;
		}
		if (useDX9)
			d3d9->Render();
		if (useDX11)
			d3d11->Render();
	}

	//ShowWindow(hwndButton, nCmdShow);
	return msg.wParam;
}