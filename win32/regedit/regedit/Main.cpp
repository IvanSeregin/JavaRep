#include "definitions.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void setupWindow(HWND hWnd, HINSTANCE hInstance);

HWND hWnd; //главное окно
HWND regeditTreeView; //контрол для отображения дерева реестра
HWND regeditListView; // контрол для отображения значений выбранной ветки
HMENU mainMenu; //меню приложения
HTREEITEM *root; //корневой элемент для TreeView
СurrentItem currItem; //структура для хранения текущего узла дерева и элемента списка
SearchResults *searchResults = new SearchResults;
HWND *htvEdit = new HWND; // редактируемый узел дерева
HWND lvEdit;
WNDPROC wpRecordProc;
HINSTANCE hInst;
TCHAR whatToSearchStr[MAX_KEY_LENGTH] = _T("");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_CTYPE, "Russian");
	WNDCLASSEX wcex;
	static TCHAR szWindowClass[] = _T("regedit");
	static TCHAR szTitle[] = _T("regedit");
	hInst = hInstance;
	
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
	
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, hInstance, NULL);
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
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		case WM_DESTROY:
		{
			delete htvEdit;
			PostQuitMessage(0);
			break;
		}
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
			//------------------------------------ ПЕРЕИМЕНОВАНИЕ КАТАЛОГА -------------------------
			if (LOWORD(wParam) == EDIT_BRANCH) //нажат пункт меню Редактировать каталог
			{
				*htvEdit = TreeView_EditLabel(regeditTreeView, currItem.currTreeNode);
			}
			else
			//------------------------------- РЕДАКТИРОВАНИЕ ИМЕНИ ПАРАМЕТРА --------------
			if (LOWORD(wParam) == EDIT_PARAM)
			{
				*htvEdit = ListView_EditLabel(regeditListView, currItem.currListItem); //шлется нотификация LVN_ENDLABELEDIT
			}
			//------------------------------- РЕДАКТИРОВАНИЕ ЗНАЧЕНИЕ ПАРАМЕТРА --------------
			if (LOWORD(wParam) == EDIT_PARAM_VALUE)
			{
				/*
				Т.к. очевидного способа отредактировать subItem у itema нет, то над subItem-ом создаем
				обычный edit, копируем в него текст в subItema и переназначаем оконную процедуру на EditProc
				Далее эта процедура занимается обработкой сообщений для editа
				*/
				//координаты для editа
				RECT *r1 = new RECT;
				//заполняем прямоугольник
				ListView_GetSubItemRect(regeditListView, currItem.currListItem, 2, LVIR_LABEL, r1);
				//номер редактируемого столбца
				int iCol = 2;
				//для текущего текста
				TCHAR pszText[MAX_KEY_LENGTH] = _T("");
				//получаем текст у текущего subItema
				ListView_GetItemText(regeditListView, currItem.currListItem, 2, pszText, MAX_KEY_LENGTH);
				//создаем edit
				lvEdit = CreateWindowEx(0,
					WC_EDIT,
					pszText,
					WS_VISIBLE | WS_CHILD | WS_BORDER,
					r1->left + 416, r1->top + 5, r1->right - r1->left-1, r1->bottom - r1->top,
					hWnd,
					(HMENU)LV_EDIT,
					(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
					NULL);
				//меням оконную процедуру
				wpRecordProc = (WNDPROC)SetWindowLong(lvEdit, GWL_WNDPROC, (LONG)EditProc);
				//Устанавливаем фокус
				SetFocus(lvEdit);
				delete r1;
			}
			else
			//------------------------------- СОЗДАНИЕ ПАРАМЕТРА REG_SZ --------------
			if (LOWORD(wParam) == ADD_REG_SZ)
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				if (renameParam(fullPath, L"Новый параметр", L"Новый параметр", L"Новое значение", L"REG_SZ") == ERROR_SUCCESS)
				{
					insertRow(regeditListView, L"Новый параметр", L"REG_SZ", L"Новое значение");
				}
				else
					MessageBox(hWnd, L"Ошибка при создании нового параметра", L"Внимание!", MB_OK);
			}
			else
			//------------------------------- СОЗДАНИЕ ПАРАМЕТРА DWORD --------------
			if (LOWORD(wParam) == ADD_REG_DWORD)
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
 				if (renameParam(fullPath, L"Новый параметр", L"Новый параметр", L"0x00000000 (0)", L"REG_DWORD") == ERROR_SUCCESS)
				{
					insertRow(regeditListView, L"Новый параметр", L"REG_DWORD", L"0x00000000 (0)");
				}
				else
					MessageBox(hWnd, L"Ошибка при создании нового параметра", L"Внимание!", MB_OK);

			}
			else
			//------------------------------------- ПОИСК В РЕЕСТРЕ-----------------------
			if (LOWORD(wParam) == FIND_FIND)
			{
				cretaeSearchDlg(hInst, hWnd);
			}
			else
			//------------------------------------- НАЙТИ ДАЛЕЕ -----------------------
			if (LOWORD(wParam) == FIND_NEXT)
			{
				whatToSearch(whatToSearchStr, true);
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
					if (((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT) //сохранение результатов редактирвоания
					{
						
						//Получаем путь до текущего каталога
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						TCHAR oldName[MAX_KEY_LENGTH] = _T("");
						TCHAR newName[MAX_KEY_LENGTH] = _T("");
						TCHAR value[MAX_KEY_LENGTH] = _T("");
						TCHAR type[TYPE_LENGTH] = _T("");

						GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);

						//Получаем новое название параметра
						GetWindowText(*htvEdit, newName, MAX_KEY_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 0, oldName, MAX_KEY_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 1, type, TYPE_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 2, value, MAX_KEY_LENGTH);
						ListView_SetItemText(regeditListView, currItem.currListItem, 0, newName);

						//Сохраняем новое название в реестре
						renameParam(fullPath, oldName, newName, value, type);
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


LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			DestroyWindow(hwnd);
			break;
		}
		case VK_RETURN:
		{
			TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
			TCHAR oldName[MAX_KEY_LENGTH] = _T("");
			TCHAR newName[MAX_KEY_LENGTH] = _T("");
			TCHAR value[MAX_KEY_LENGTH] = _T("");
			TCHAR type[TYPE_LENGTH] = _T("");
			ListView_GetItemText(regeditListView, currItem.currListItem, 0, oldName, MAX_KEY_LENGTH);
			ListView_GetItemText(regeditListView, currItem.currListItem, 1, type, TYPE_LENGTH);
			//ListView_GetItemText(regeditListView, currItem.currListItem, 2, value, MAX_KEY_LENGTH);

			GetWindowText(lvEdit, value, MAX_KEY_LENGTH);
			//Получаем путь до текущего каталога
			GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
			//переименовываем параметр в списке
			ListView_SetItemText(regeditListView, currItem.currListItem, 2, value);
			//удалем едит бокс
			DestroyWindow(hwnd);
			//Сохраняем новое название в реестре
			renameParam(fullPath, oldName, oldName, value, type);
			break;
		}
		}
	}
	}
	return CallWindowProc(wpRecordProc, hwnd, message, wParam, lParam);
}

//функция подготавливает структуру SearchResults для поиска
void whatToSearch(TCHAR strToSearch[MAX_KEY_LENGTH], bool resumeSearch)
{
	if (!resumeSearch)
	{
		wcscpy(whatToSearchStr, strToSearch);
		initSearch(searchResults);
	}
	std::stack <TCHAR*> tmp;
	if (searchInReg(strToSearch, searchResults, &tmp))
	{
		//Разворачиваем элементы дерева по найденному пути, который находитя в SearchResults.path
		//Создаем реверсивную копию стэка SearchResults.path
		std::stack <TCHAR*> tmpRevers;
		TCHAR *str;
		TCHAR *pszText = new TCHAR[MAX_KEY_LENGTH];
		while (!tmp.empty())
		{
			str = tmp.top();
			tmpRevers.push(str);
			tmp.pop();
		}
		
		HTREEITEM tophItem;
		HTREEITEM childHItem;
		tophItem = TreeView_GetRoot(regeditTreeView);

		while (!tmpRevers.empty())
		{
			str = tmpRevers.top();
			tmpRevers.pop();
			childHItem = TreeView_GetNextItem(regeditTreeView, tophItem, TVGN_CHILD);
			while (childHItem != NULL)
			{
				TVITEM tmpTVItem = { 0 };

				tmpTVItem.mask = TVIF_TEXT;
				tmpTVItem.pszText = pszText;
				tmpTVItem.cchTextMax = MAX_KEY_LENGTH;
				tmpTVItem.hItem = childHItem;
				TreeView_GetItem(regeditTreeView, &tmpTVItem);
				if (wcsstr(tmpTVItem.pszText, str) && wcsstr(str, tmpTVItem.pszText))
				{
					TreeView_Expand(regeditTreeView, childHItem, TVE_EXPAND);
					tophItem = childHItem;
					break;
				}
				childHItem = TreeView_GetNextItem(regeditTreeView, childHItem, TVGN_NEXT);
			}
			//delete[] str;
		}
		SetFocus(regeditTreeView);
		TreeView_Select(regeditTreeView, tophItem, TVGN_CARET);
		TreeView_SetItemState(regeditTreeView, tophItem, TVIS_SELECTED, TVIS_SELECTED);
		delete[] pszText;
	}
	else
	{
		MessageBox(hWnd, L"Строка не найдена!", L"Внимание!", 0);
		//проводим инициализацию поисковой структуры
		//с целью освобождения памяти и подготовки ее к новому поиску
		initSearch(searchResults);
	}
}