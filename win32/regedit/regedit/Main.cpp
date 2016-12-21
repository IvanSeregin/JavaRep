//https://www.napishem.com/customer-248717.html


#include "definitions.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setupWindow(HWND hWnd, HINSTANCE hInstance);

HWND regeditTreeView; //контрол для отображения дерева реестра
HWND regeditListView; // контрол для отображения значений выбранной ветки
HMENU mainMenu; //меню приложения
HTREEITEM *root; //корневой элемент для TreeView
СurrentItem currItem; //структура для хранения текущего узла дерева и элемента списка
HWND *htvEdit = new HWND; // редактируемый узел дерева

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_CTYPE, "Russian");
	WNDCLASSEX wcex;
	static TCHAR szWindowClass[] = _T("regedit");
	static TCHAR szTitle[] = _T("regedit");
	//htvEdit = NULL;
	
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
	
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, hInstance, NULL);
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
			delete htvEdit;
			PostQuitMessage(0);
			break;
		case WM_COMMAND: 
		{
			//---------------------------------------------------- ВЫХОД ---------------------------
			if (LOWORD(wParam) == FILE_EXIT) //нажат пункт меню Выход
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			} else
			//---------------------------------------------------- СОЗДАНИЕ ДАМПА ------------------
			if (LOWORD(wParam) == FILE_DUMP) //нажат пункт меню Создать дамп
			{
				dumpRegistry(); //вызов функции создания дампа
			} else
			//---------------------------------------------------- СОЗДАНИЕ ДАМПА ВЕТКИ------------------
			if (LOWORD(wParam) == FILE_SAVE_CURR) //нажат пункт меню Создать дамп
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				dumBranch(fullPath); //вызов функции создания дампа
			}
			else
			//------------------------------------ ПЕРЕИМЕНОВАНИЕ КАТАЛОГА -------------------------
			if (LOWORD(wParam) == EDIT_BRANCH) //нажат пункт меню Редактировать каталог
			{
				*htvEdit = TreeView_EditLabel(regeditTreeView, currItem.currTreeNode);
			}
			else
			//---------------------------------------------------- УДАЛЕНИЕ ПАРАМЕТРА --------------
			if (LOWORD(wParam) == DEL_PARAM) //нажат пункт Удалить параметр
			{
				if (currItem.currListItem == -1)
				{
					MessageBox(hWnd, L"Не выбран ни один параметр", L"OK", MB_OK);
					break;
				}
				//Получаем путь до текущего параметра ключа
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				//Получаем имя текущего параметра ключа
				TCHAR pszText[MAX_KEY_LENGTH];
				ListView_GetItemText(regeditListView, currItem.currListItem, 0, pszText, MAX_KEY_LENGTH);
				if (deleteParam(fullPath, pszText) != ERROR_SUCCESS)
				{
					MessageBox(hWnd, L"Ошибка при удалении параметра", L"OK", MB_OK);
				}
				else
				{
					NMHDR nmh;
					nmh.code = TVN_SELCHANGED;
					nmh.idFrom = GetDlgCtrlID(regeditTreeView);
					nmh.hwndFrom = regeditTreeView;
					
					NM_TREEVIEW t;
					t.action = TVN_SELCHANGED;
					t.hdr = nmh;
					t.itemNew.hItem = currItem.currTreeNode;
					SendMessage(hWnd, WM_NOTIFY, LOWORD(REG_TREE_VIEW), (LPARAM)&t);
					MessageBox(hWnd, L"Параметр удален успешно", L"OK", MB_OK);
				}
			} else
			//---------------------------------------------------- УДАЛЕНИЕ ВЕТКИ РЕЕСТРА --------------
			if (LOWORD(wParam) == DEL_BRANCH) //нажат пункт Удалить ветку
			{
				//Получаем путь до текущего параметра ключа
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				if (deleteBranch(fullPath) != ERROR_SUCCESS)
				{
					MessageBox(hWnd, L"Ошибка при удалении ветки", L"OK", MB_OK);
				}
				else
				{
					//Обновляем ветку- родителя
					NMHDR nmh;
					nmh.code = TVN_ITEMEXPANDING;
					nmh.idFrom = GetDlgCtrlID(regeditTreeView);
					nmh.hwndFrom = regeditTreeView;

					NM_TREEVIEW t;
					t.action = TVE_EXPAND;
					t.hdr = nmh;
					t.itemNew.hItem = TreeView_GetParent(regeditTreeView, currItem.currTreeNode);
					currItem.currTreeNode = t.itemNew.hItem;
					SendMessage(hWnd, WM_NOTIFY, LOWORD(REG_TREE_VIEW), (LPARAM)&t);
					MessageBox(hWnd, L"Ветка удалена успешно", L"OK", MB_OK);

				}				
			}
			//--------------------------------------ДОБАВЛЕНИЕ КАТАЛОГА -----------------------
			if (LOWORD(wParam) == ADD_BRANCH)
			{
				//Получаем путь до текущего каталога
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				//Пробуем добавить новый каталог, если все ок, то добавляем также узел в дерево
				if (addBranch(fullPath, L"Новый раздел") == ERROR_SUCCESS)
				{
					insertInTreeView(hWnd, currItem.currTreeNode, L"Новый раздел");
				}
				else
					MessageBox(hWnd, L"Невозможно создать раздел", L"OK", MB_OK);
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
				case REG_TREE_VIEW: // события связанные с контролом TreeView
				{
					//--------------------- событие на выделение пункта в списке
					if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
					{
						HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem; //получаем указатель на выделенную строку
						TCHAR fullPath[MAX_KEY_LENGTH]=_T("");
						/*Если выделена строка отличная от корня
						то находим полный путь в реестре до этого каталога
						и перечисляем все ключи и их значения в контроле ListView
						*/
						if (hClicked != *root) 
						{
							GetFullPath(hClicked, root, regeditTreeView, fullPath);
							currItem.currTreeNode = hClicked;
							enumKeys(regeditListView, fullPath);
						}
					}
					//---------------- //событие на разворачивание пункта в списке
					else if (((LPNMHDR)lParam)->code == TVN_ITEMEXPANDING) 
					{
						HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem;
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						if (((NMTREEVIEW*)lParam)->action == TVE_EXPAND)
						{
							if (hClicked != *root)
							{
								//получаем путь до каталога
								GetFullPath(hClicked, root, regeditTreeView, fullPath);
								//предварительно очищаем потомков текущего узла в дереве
								clearBranch(regeditTreeView, ((NMTREEVIEW*)lParam)->itemNew.hItem);
								//обновляем содержимое узла дерева
								updateSubCatalogs(hWnd, ((NMTREEVIEW*)lParam)->itemNew, fullPath);
							}
							else 
								break;
						}
					}
					//------------------- событие при изменении узла дерева----------------------------------
					else if (((LPNMHDR)lParam)->code == TVN_ENDLABELEDIT)
					{
						//Получаем путь до текущего каталога
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
						//переименовываем узел дерева
						TV_ITEM currTVItem = getCurrentItem(regeditTreeView, currItem.currTreeNode);
						TCHAR newLabelText[MAX_KEY_LENGTH] = _T("");
						GetWindowText(*htvEdit, newLabelText, MAX_KEY_LENGTH);
						wcscpy(currTVItem.pszText, newLabelText);
						TreeView_SetItem(regeditTreeView, &currTVItem);
						//изменяем в реестре
						renameBranch(fullPath, newLabelText);
					}
					break;
				}
				case REG_LIST_VIEW:// события связанные с контролом ListView
				{
					if (((LPNMHDR)lParam)->code == NM_CLICK)//событие на выделение пункта в списке ListView
					{
						//номер выделенной строки
						LPNMITEMACTIVATE  lpnmitem = (LPNMITEMACTIVATE)lParam; //указатель на текущую позицию в списке
						currItem.currListItem = lpnmitem->iItem;

						//Получить текст строки
						//TCHAR pszText[MAX_KEY_LENGTH];
						//ListView_GetItemText(regeditListView, currItem.currListItem, 0, pszText, MAX_KEY_LENGTH);

					}
					break;
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