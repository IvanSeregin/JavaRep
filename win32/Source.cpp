#include <windows.h>
#include <CommCtrl.h>

#define MWND_WIDTH  1024
#define MWND_HEIGHT 600
#define IDC_TREE 0x1000
#define IDC_LIST 0x1001

HTREEITEM __stdcall InitRoot(HWND hWndMain, HWND hTreeView);
HWND __stdcall CreateTreeViewCtrl(HWND hWndMain);
HWND __stdcall CreateListViewCtrl(HWND hMainWnd);
LRESULT __stdcall WndProc(_In_  HWND hwnd, _In_  UINT uMsg, _In_  WPARAM wParam, _In_  LPARAM lParam);
void __stdcall UpdateExpandedInfo(HWND hTreeView, TV_ITEMW Parent);
void __stdcall GetFullPath(HTREEITEM hItem, HWND hTreeView, LPWSTR buf);
void __stdcall ListDirectory(HWND hListView, LPWSTR path);


HINSTANCE g_hInstance;
HWND g_hWndMain;
HWND g_hTreeView;
HWND g_hListView;

LPWSTR g_rootName = L"Компьютер";
LPWSTR g_col1Name = L"Файл";
LPWSTR g_col2Name = L"Тип";
HTREEITEM g_hRoot;

WCHAR activePath[_MAX_PATH];

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	LPCWSTR className = L"TreeControlSex";
	LPCWSTR appName = L"TreeViewSex Test";
	WNDCLASSEXW wnd = {0};

	MSG msg;
	INITCOMMONCONTROLSEX icc = {0};

	g_hInstance = GetModuleHandle(NULL);

	wnd.cbSize = sizeof(WNDCLASSEXW);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance; 
	wnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(243,243,247));
	wnd.hIcon = LoadIconW(hInstance, IDI_APPLICATION);
	wnd.hIconSm = NULL;
	wnd.hCursor = LoadCursorW(NULL,IDC_ARROW);
	wnd.lpszClassName = className;
	wnd.style = CS_HREDRAW|CS_VREDRAW;
	wnd.lpszMenuName = NULL;
	wnd.lpfnWndProc = WndProc;

	RegisterClassExW(&wnd);

	g_hWndMain=CreateWindowW(className, appName, WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT, MWND_WIDTH, MWND_HEIGHT, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hWndMain, SW_SHOWNORMAL);
	UpdateWindow(g_hWndMain);

	icc.dwSize=sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC=ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT __stdcall WndProc(_In_  HWND hwnd, _In_  UINT uMsg, _In_  WPARAM wParam, _In_  LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			g_hTreeView = CreateTreeViewCtrl(hwnd);
			g_hRoot = InitRoot(hwnd, g_hTreeView);

			g_hListView = CreateListViewCtrl(hwnd);
			return 0;
		}
	case WM_PAINT:
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_NOTIFY:
		switch (wParam)
		{
		case IDC_TREE:
			{
				switch (((NMHDR*)lParam)->code)
				{
				case TVN_ITEMEXPANDING:
					if(((NMTREEVIEW*)lParam)->action == TVE_EXPAND)
					{
						UpdateExpandedInfo(g_hTreeView,((NMTREEVIEW*)lParam)->itemNew);
					}
					return 0;
				case TVN_SELCHANGED:
					{
						HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem;
						if(hClicked != g_hRoot)
						{
							GetFullPath(((NMTREEVIEW*)lParam)->itemNew.hItem, g_hTreeView, activePath);
							ListDirectory(g_hListView, activePath);
						}
						else
						{
							ListView_DeleteAllItems(g_hListView);
						}
					}			
					return 0;
				}
			}
			break;
		}
		break;
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

HWND __stdcall CreateTreeViewCtrl(HWND hWndMain)
{
	RECT rc;
	GetClientRect(hWndMain, &rc);
	
	return CreateWindowEx(0, WC_TREEVIEWW, NULL, 
		WS_VISIBLE|WS_CHILD|TVS_HASLINES|TVS_HASBUTTONS|
		TVS_LINESATROOT|TVS_SHOWSELALWAYS|TVS_DISABLEDRAGDROP|WS_BORDER,
		10, 10, rc.right/3, rc.bottom - 20, hWndMain, (HMENU)IDC_TREE, g_hInstance, 0 );
}

void __stdcall ClearBranch(HWND hTreeView,volatile HTREEITEM hParent)
{
	HTREEITEM hSibling = TreeView_GetNextSibling(hTreeView, hParent);
	if(hSibling)
		ClearBranch(hTreeView, hSibling);
	HTREEITEM hChild = TreeView_GetChild(hTreeView, hParent);
	if(hChild)
		ClearBranch(hTreeView, hChild);
	TreeView_DeleteItem(hTreeView, hParent);

}

void __stdcall ClearSubItems(HWND hTreeView, HTREEITEM hParent)
{
	HTREEITEM hChild;
	if(hChild = TreeView_GetChild(hTreeView, hParent))
	{	
		ClearBranch(hTreeView, hChild);
	}
}

void __stdcall GetFullPath(HTREEITEM hItem, HWND hTreeView, LPWSTR buf)
{
	TV_ITEM item = {0};
	WCHAR tmppath[_MAX_PATH];
	WCHAR tmpoverlap[_MAX_PATH];
	HTREEITEM tmphItem;

	item.mask = TVIF_TEXT;
	item.pszText = tmppath;
	item.cchTextMax = _MAX_PATH;
	item.hItem = hItem;
	TreeView_GetItem(hTreeView, &item);

	wsprintf(buf, L"%s%s", item.pszText, L"\\");

	while ((hItem = TreeView_GetParent(hTreeView, hItem)) != g_hRoot)
	{
		item.mask = TVIF_TEXT;
		item.pszText = tmppath;
		item.cchTextMax = _MAX_PATH;
		item.hItem = hItem;
		TreeView_GetItem(hTreeView, &item);

		wcscpy(tmpoverlap, buf);
		wsprintf(buf, L"%s%s%s", item.pszText, L"\\", tmpoverlap);
	}
	wcscat(buf, L"*");
}

inline BOOL IsDirectory(PWIN32_FIND_DATA wfd)
{
	return (wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(wfd->cFileName, L".") && wcscmp(wfd->cFileName, L".."));
}


void __stdcall InsertDirectories(PWIN32_FIND_DATA wfd, HTREEITEM hParent, HWND hTreeView)
{
	TV_ITEMW item = {0};
	TV_INSERTSTRUCTW insStrct= {0};

	WCHAR tmp[_MAX_PATH];

	WIN32_FIND_DATA fd;
	BOOL HaveSub = 0;

	if(IsDirectory(wfd))
	{
		GetFullPath(hParent, hTreeView, tmp);
		tmp[wcslen(tmp)-1]=0;
		wcscat(tmp, wfd->cFileName);
		wcscat(tmp, L"\\*");

		HANDLE hFirst;

		if((hFirst = FindFirstFileW(tmp, &fd))!=INVALID_HANDLE_VALUE)
		{
			HaveSub = IsDirectory(&fd);
			while(FindNextFileW(hFirst, &fd) && !HaveSub)
				HaveSub = IsDirectory(&fd);
		}

		item.mask = TVIF_TEXT|TVIF_CHILDREN;
		item.cchTextMax = wcslen(wfd->cFileName) + 1;
		item.pszText = wfd->cFileName;
		item.cChildren = HaveSub;

		insStrct.hParent = hParent;
		insStrct.item = item;
		insStrct.hInsertAfter = TVI_SORT;
		TreeView_InsertItem(hTreeView, &insStrct);
	}
}

void __stdcall UpdateExpandedInfo(HWND hTreeView, TV_ITEMW Parent)
{
	WCHAR name[_MAX_PATH];
	if(Parent.hItem == g_hRoot)
		return;	

	ClearSubItems(hTreeView, Parent.hItem);

	GetFullPath(Parent.hItem, hTreeView, name);

	WIN32_FIND_DATA wfd;
	HANDLE hFirst;
	
	if((hFirst= FindFirstFileW(name, &wfd))!=INVALID_HANDLE_VALUE)
	{
		InsertDirectories(&wfd, Parent.hItem, hTreeView);
		while(FindNextFileW(hFirst, &wfd))
			InsertDirectories(&wfd, Parent.hItem, hTreeView);
	}
}

HTREEITEM __stdcall InitRoot(HWND hWndMain, HWND hTreeView)
{
	TV_ITEMW item = {0};
	TV_INSERTSTRUCTW insStrct = {0};

	item.mask = TVIF_TEXT|TVIF_CHILDREN;
	item.cchTextMax = wcslen(g_rootName);
	item.pszText = g_rootName;
	item.cChildren = 1;

	insStrct.item = item;
	insStrct.hInsertAfter = TVI_ROOT;

	HTREEITEM hRoot = TreeView_InsertItem(hTreeView, &insStrct);

	DWORD drives = GetLogicalDrives();
	WCHAR path[] = L"x:\\";

	for(WCHAR c = L'A'; c <= L'Z'; c++, drives>>=1)
	{
		if(drives & 1)
		{
			path[0]=c;
			if(GetDriveTypeW(path) & DRIVE_FIXED)
			{
				path[2]=0;				
				item.cchTextMax = wcslen(path+1);
				item.mask=TVIF_TEXT|TVIF_CHILDREN;
				item.pszText = path;

				insStrct.item=item;
				insStrct.hParent = hRoot;
				insStrct.hInsertAfter = TVI_SORT;
				HTREEITEM x = TreeView_InsertItem(hTreeView, &insStrct);
			}
		}
	}
	return hRoot;
}

HWND __stdcall CreateListViewCtrl(HWND hMainWnd)
{
	RECT mWndRC;
	RECT trvRC;

	GetClientRect(hMainWnd, &mWndRC);
	GetClientRect(g_hTreeView, &trvRC);

	HWND hlistView = CreateWindowEx(0, WC_LISTVIEWW, L"", WS_CHILD|WS_VISIBLE|LVS_REPORT|WS_BORDER, 
		trvRC.right + 20, mWndRC.top + 10, mWndRC.right - trvRC.right -20, mWndRC.bottom - 20, 
		hMainWnd, (HMENU)IDC_LIST, g_hInstance, 0 );
	ListView_SetExtendedListViewStyle(hlistView,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	LVCOLUMN col;
	col.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH|LVCF_SUBITEM;
	col.pszText = g_col1Name;
	col.iSubItem=0;
	col.cchTextMax = wcslen(g_col1Name)+1;
	col.fmt = LVCFMT_LEFT;
	col.cx = trvRC.right - 15;

	ListView_InsertColumn(hlistView, 0, &col);
	col.pszText = g_col2Name;
	col.cchTextMax = wcslen(g_col2Name)+1;
	col.cx = trvRC.right - 50;
	ListView_InsertColumn(hlistView, 1, &col);

	return hlistView;
}

void __stdcall InsertRows(HWND hlistView, PWIN32_FIND_DATA wfd)
{

	LVITEM lvi, subLvi;
	lvi.mask = LVIF_TEXT;
	subLvi.mask = LVIF_TEXT;

	lvi.iSubItem = 0;
	lvi.iItem = 0;
	lvi.pszText = wfd->cFileName;
	lvi.cchTextMax = wcslen(wfd->cFileName)+1;
	ListView_InsertItem(hlistView, &lvi);

	subLvi.pszText = IsDirectory(wfd)?L"Директория":L"Файл";
	subLvi.cchTextMax = 11;
	subLvi.iSubItem = 1;
	subLvi.iItem = 0;
	ListView_SetItem(hlistView, &subLvi);
}

void __stdcall ListDirectory(HWND hListView, LPWSTR path)
{
	HANDLE hFirst;
	WIN32_FIND_DATA wfd;

	ListView_DeleteAllItems(hListView);

	if((hFirst= FindFirstFileW(path, &wfd))!=INVALID_HANDLE_VALUE)
	{
		InsertRows(hListView, &wfd);

		while(FindNextFileW(hFirst, &wfd))
			InsertRows(hListView, &wfd);
	}
}