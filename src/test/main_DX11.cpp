/*
TODO:
- init d3d9
- init d3d10
- init d3d11
- init d3d12
*/

#include "stdafx.h"
#include "D3D11.h"

void HandleLastError(void);
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
	auto d3d11 = make_unique<D3D11>(hWnd);
	MSG msg;
	while ( 1 ) {
		while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ( msg.message == WM_QUIT ) {
			break;
		}
		d3d11->Render();
	}
	return msg.wParam;
}