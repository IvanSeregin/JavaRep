#include "definitions.h"
#include "resource.h"

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

HWND createRegeditTreeView(HWND hWnd, HINSTANCE hInstance)
{
	HWND regeditTreeView = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("������"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT,
		5, 5,
		400, 810,
		hWnd,
		(HMENU)REG_TREE_VIEW,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	/*
	HIMAGELIST hImageList;
	hImageList = ImageList_Create(16, 16, ILC_COLOR16, 2, 10);
	HBITMAP hBitMap;   // bitmap handler
					   // load the picture from the resource
	hBitMap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	ImageList_Add(hImageList, hBitMap, NULL);
	SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	*/

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