//https://www.napishem.com/customer-248717.html


#include "definitions.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setupWindow(HWND hWnd, HINSTANCE hInstance);

HWND regeditTreeView; //контрол для отображения дерева реестра
HWND regeditListView; // контрол для отображения значений выбранной ветки
HMENU mainMenu; //меню приложения
HTREEITEM *root; //корневой элемент для TreeView

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_CTYPE, "Russian");
	WNDCLASSEX wcex;
	static TCHAR szWindowClass[] = _T("regedit");
	static TCHAR szTitle[] = _T("regedit");
	
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
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("regedit"), NULL);
		return 1;
	}
	
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 900, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("regedit"), NULL);
		return 1;
	}

	setupWindow(hWnd, hInstance); //создаем контролы в окне
	root = new HTREEITEM;
	loadRegistry(hWnd, regeditTreeView, root); //загружаем реестр 

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
	switch (message)
	{
		case WM_PAINT:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND: 
		{
			if (LOWORD(wParam) == FILE_EXIT) //нажат пункт меню Выход
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			if (LOWORD(wParam) == FILE_DUMP) //нажат пункт меню Создать дамп
			{
				dumpRegistry(); //вызов функции создания дампа
			}
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
		case WM_NOTIFY:
		{
			switch (LOWORD(wParam))
			{
			case REG_TREE_VIEW:
				if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
				{
					HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem;
					TCHAR regPath[MAX_KEY_LENGTH];
					if (hClicked != *root)
					{
						GetFullPath(((NMTREEVIEW*)lParam)->itemNew.hItem, root, regeditTreeView, regPath);
						//ListDirectory(g_hListView, activePath);
					}
					//MessageBox(hWnd, regPath, L"OK", MB_OK);

				}
				else if (((LPNMHDR)lParam)->code == TVN_ITEMEXPANDING)
				{
					TCHAR fullPath[MAX_KEY_LENGTH];
					if (((NMTREEVIEW*)lParam)->action == TVE_EXPAND)
					{
						GetFullPath(((NMTREEVIEW*)lParam)->itemNew.hItem, root, regeditTreeView, fullPath);
						updateSubCatalogs(regeditTreeView, ((NMTREEVIEW*)lParam)->itemNew.hItem, fullPath);
					}
				}
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}
	return 0;
}

void setupWindow(HWND hWnd, HINSTANCE hInstance)
{
	regeditTreeView = createRegeditTreeView(hWnd, hInstance);
	regeditListView = createRegeditListView(hWnd);
	mainMenu = createMenu(hWnd);
}

