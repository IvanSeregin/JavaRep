#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h>  
#include <CommCtrl.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwndButton = NULL;
HWND hwndBtnCancel = NULL;


#define BTN_OK  100
#define BTN_CANCEL 101
#define ID_TREEVIEW 102

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex; 
	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style = CS_HREDRAW | CS_VREDRAW; 
	wcex.lpfnWndProc = WndProc; 
	wcex.cbClsExtra = 0; 
	wcex.cbWndExtra = 0; 
	wcex.hInstance = hInstance; 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); 
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
	wcex.lpszMenuName = NULL; 
	wcex.lpszClassName = szWindowClass; 
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex)) 
	{ 
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL); 
		return 1; 
	}

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, hInstance, NULL); 
	if (!hWnd) 
	{ 
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL); 
		return 1; 
	}

	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		100,        // Button width
		25,        // Button height
		hWnd,     // Parent window
		(HMENU)BTN_OK,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	HWND hwndBtnCancel = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Cancel",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		130,         // x position 
		10,         // y position 
		100,        // Button width
		25,        // Button height
		hWnd,     // Parent window
		(HMENU)BTN_CANCEL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	HWND hwndTV = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
		220,
		0,
		200,
		200,
		hWnd,
		(HMENU)ID_TREEVIEW,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);

	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0)) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 
	
	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps; 
	HDC hdc; 
	TCHAR greeting[] = _T("Hello, World!"); 
	switch (message) 
	{
		case WM_PAINT: 
			hdc = BeginPaint(hWnd, &ps); 
			// Here your application is laid out. 
			// For this introduction, we just print out "Hello, World!" 
			// in the top left corner. 
			TextOut(hdc, 5, 5, greeting, _tcslen(greeting)); 
			// End application specific layout section. 
			EndPaint(hWnd, &ps); 
			break; 
		case WM_DESTROY: 
			PostQuitMessage(0); 
			break; 
		case WM_COMMAND: {
			if (LOWORD(wParam) == BTN_OK) 
			{
				MessageBox(hWnd, L"OK was clicked", L"OK", MB_OK);
			}
			else
			if (LOWORD(wParam) == BTN_CANCEL)
			{
				MessageBox(hWnd, L"Cancel was clicked", L"OK", MB_OK);
			}

			break;
		}

		default: 
			return DefWindowProc(hWnd, message, wParam, lParam); 
			break; 
	} 
	return 0; 
} 

