#include "SearchDialog.h"
LRESULT CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC mainProc;
HWND DlgWin;

void cretaeSearchDlg(HINSTANCE hInstance, HWND hWnd)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc, 0);
	/*
	DlgWin = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
	mainProc = (WNDPROC)SetWindowLong(DlgWin, GWL_WNDPROC, (LONG)DialogProc);
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

LRESULT CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		return DefWindowProc(hwndDlg, message, wParam, lParam);
		break;
	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
		return 1;
	}
	}
	//return CallWindowProc(mainProc, hwndDlg, message, wParam, lParam);
}