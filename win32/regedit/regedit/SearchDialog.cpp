#include "SearchDialog.h"
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND DlgWin;

void cretaeSearchDlg(HINSTANCE hInstance, HWND hWnd)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc, 0);
	/*
	DlgWin = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL2), hWnd, (DLGPROC)DialogProc);
	
	ShowWindow(DlgWin, SW_SHOW);
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		if (!IsWindow(DlgWin) || !IsDialogMessage(DlgWin, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	*/
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		return DefWindowProc(hwndDlg, message, wParam, lParam);
		break;
	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
		return true;
	}
	}
	return TRUE;
}