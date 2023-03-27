#include <Windows.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"F Pressed");
			break;
		}
	}
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Defining Engine Name pClassName
	LPCWSTR pClassName = L"Elektron Engine V2.0";

	// Declaring Window Class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	// Register Window Class
	RegisterClassEx(&wc);

	// Creating Instance of Window
	HWND hWnd = CreateWindowEx(0, 
		pClassName,
		pClassName, 
		WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
		0, 0, 600, 600,
		nullptr, nullptr,
		hInstance,
		nullptr);

	// Show Window using Window Handle hWnd
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	BOOL gResult;
	while ((gResult=GetMessage(&msg,nullptr,0,0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) return -1;
	else return msg.wParam;
}