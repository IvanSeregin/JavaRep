#include "layout.h"

HWND createBtn(HWND hWnd)
{
	HWND hwndBtnCancel = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Cancel",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		390,         // x position 
		10,         // y position 
		100,        // Button width
		25,        // Button height
		hWnd,     // Parent window
		(HMENU)BTN_CANCEL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return hwndBtnCancel;
}
HMENU createMenu(HWND hWnd)
{
	HMENU mainMenu = CreateMenu();
	HMENU PopMenuFile = CreatePopupMenu();
	HMENU PopMenuFind = CreatePopupMenu();
	HMENU PopMenuEdit = CreatePopupMenu();
	HMENU PopMenuEditObject = CreatePopupMenu();

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuFile, L"&����");
	AppendMenu(PopMenuFile, MF_STRING, FILE_DUMP, L"������� ����");
	AppendMenu(PopMenuFile, MF_STRING, FILE_SAVE_CURR, L"��������� �����");
	AppendMenu(PopMenuFile, MF_STRING, FILE_EXIT, L"�����");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuFind, L"&�����");
	AppendMenu(PopMenuFind, MF_STRING, FIND_FIND, L"�����");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuEdit, L"&��������������");
	AppendMenu(PopMenuEdit, MF_STRING | MF_POPUP, (UINT)PopMenuEditObject, L"�������������");
	AppendMenu(PopMenuEditObject, MF_STRING, EDIT_BRANCH, L"������������� �����");
	AppendMenu(PopMenuEditObject, MF_STRING, EDIT_KEY, L"�������� ����");
	AppendMenu(PopMenuEdit, MF_STRING, EDIT_ADD, L"��������");
	AppendMenu(PopMenuEdit, MF_STRING, EDIT_DEL, L"�������");

	SetMenu(hWnd, mainMenu);
	SetMenu(hWnd, PopMenuFile);
	SetMenu(hWnd, PopMenuFind);
	SetMenu(hWnd, PopMenuEdit);
	SetMenu(hWnd, PopMenuEditObject);

	return mainMenu;
}

HWND createRegeditTreeView(HWND hWnd)
{
	HWND regeditTreeView = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("������"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
		5, 5,
		400, 810,
		hWnd,
		(HMENU)REG_TREE_VIEW,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return regeditTreeView;
}

HWND createRegeditListView(HWND hWnd)
{
	// Create the list-view window in report view with label editing enabled.
	HWND regeditListView = CreateWindowEx(0,
		WC_LISTVIEW,
		TEXT("������"),
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_BORDER,
		415, 5,
		400, 810,
		hWnd,
		(HMENU)REG_LIST_VIEW,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return regeditListView;
}