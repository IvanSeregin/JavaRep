#include "SearchDialog.h"
HWND mainWin;
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

int cretaeSearchDlg(HINSTANCE hInstance, HWND hWnd)
{
	mainWin = hWnd;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc, 0);
	return 0;
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, ERROR_SUCCESS);
			return TRUE;
		}
		else
		if (LOWORD(wParam) == IDOK)
		{
			HWND edit = GetDlgItem(hwndDlg, IDC_EDIT1);
			TCHAR text[MAX_KEY_LENGTH] = L"";
			GetWindowText(edit, text, MAX_KEY_LENGTH);
			EndDialog(hwndDlg, ERROR_SUCCESS);
			whatToSearch(text);			
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}
