#include "definitions.h"
#include "resource.h"

HMENU createMenu(HWND hWnd)
{
	HMENU mainMenu = CreateMenu();
	HMENU PopMenuFile = CreatePopupMenu();
	HMENU PopMenuFind = CreatePopupMenu();
	HMENU PopMenuEdit = CreatePopupMenu();
	HMENU PopMenuEditObject = CreatePopupMenu();
	HMENU PopMenuAddObject = CreatePopupMenu();
	HMENU PopMenuAddREG_SZ = CreatePopupMenu();
	HMENU PopMenuAddREG_DWORD = CreatePopupMenu();
	HMENU PopMenuAddParam = CreatePopupMenu();
	HMENU PopMenuDelObject = CreatePopupMenu();

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuFile, L"&Файл");
	AppendMenu(PopMenuFile, MF_STRING, FILE_DUMP, L"Создать дамп");
	AppendMenu(PopMenuFile, MF_STRING, FILE_SAVE_CURR, L"Сохранить ветку");
	AppendMenu(PopMenuFile, MF_STRING, FILE_EXIT, L"Выход");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuFind, L"&Поиск");
	AppendMenu(PopMenuFind, MF_STRING, FIND_FIND, L"Найти");
	AppendMenu(PopMenuFind, MF_STRING, FIND_NEXT, L"Найти далее");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)PopMenuEdit, L"&Редактирование");
	AppendMenu(PopMenuEdit, MF_STRING | MF_POPUP, (UINT)PopMenuEditObject, L"Редактировать");
	AppendMenu(PopMenuEdit, MF_STRING | MF_POPUP, (UINT)PopMenuAddObject, L"Добавить");
	AppendMenu(PopMenuEdit, MF_STRING | MF_POPUP, (UINT)PopMenuDelObject, L"Удалить");
	AppendMenu(PopMenuEditObject, MF_STRING, EDIT_BRANCH, L"Переименовать папку");
	AppendMenu(PopMenuEditObject, MF_STRING, EDIT_PARAM, L"Изменить имя параметра");
	AppendMenu(PopMenuEditObject, MF_STRING, EDIT_PARAM_VALUE, L"Изменить значение параметра");
	AppendMenu(PopMenuAddObject, MF_STRING, ADD_BRANCH, L"Добавить папку");
	AppendMenu(PopMenuAddObject, MF_STRING | MF_POPUP, (UINT)PopMenuAddParam, L"Добавить параметр");
	AppendMenu(PopMenuAddParam, MF_STRING, ADD_REG_SZ, L"Добавить REG_SZ параметр");
	AppendMenu(PopMenuAddParam, MF_STRING, ADD_REG_DWORD, L"Добавить REG_DWORD параметр");
	AppendMenu(PopMenuDelObject, MF_STRING, DEL_BRANCH, L"Удалить папку");
	AppendMenu(PopMenuDelObject, MF_STRING, DEL_PARAM, L"Удалить ключ");

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
		TEXT("Реестр"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT | TVS_EDITLABELS,
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
	HWND regeditListView = CreateWindowEx(0,
		WC_LISTVIEW,
		TEXT("Реестр"),
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_BORDER,
		415, 5,
		700, 810,
		hWnd,
		(HMENU)REG_LIST_VIEW,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	ListView_SetExtendedListViewStyle(regeditListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN col;
	col.iSubItem = 0;
	col.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;

	col.pszText = COL_NAME;
	col.cchTextMax = wcslen(COL_NAME) + 1;
	col.cx = 200;
	ListView_InsertColumn(regeditListView, 0, &col);
	
	col.pszText = COL_TYPE;
	col.cchTextMax = wcslen(COL_TYPE) + 1;
	col.cx = 100;
	ListView_InsertColumn(regeditListView, 1, &col);
	
	col.pszText = COL_VALUE;
	col.cchTextMax = wcslen(COL_VALUE) + 1;
	col.cx = 400;
	ListView_InsertColumn(regeditListView, 2, &col);
	
	return regeditListView;
}