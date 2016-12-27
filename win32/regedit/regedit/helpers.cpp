#include "definitions.h"
void removeSubStr(TCHAR str[MAX_KEY_LENGTH], TCHAR substr[MAX_KEY_LENGTH]);
void ClearSubBranches(HWND hTreeView, HTREEITEM hItem);
//функция получения полного пути до каталога реестра, который представлен
//в TreeView как hItem
void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf)
{
	TV_ITEM item = { 0 };
	TCHAR tmppath[MAX_KEY_LENGTH];
	TCHAR tmpoverlap[MAX_KEY_LENGTH];
	HTREEITEM tmphItem;

	item.mask = TVIF_TEXT;
	item.pszText = tmppath;
	item.cchTextMax = MAX_KEY_LENGTH;
	item.hItem = hItem;
	TreeView_GetItem(hTreeView, &item);

	wsprintf(buf, L"%s%s", item.pszText, L"\\");

	while ((hItem = TreeView_GetParent(hTreeView, hItem)) != *root)
	{
		item.mask = TVIF_TEXT;
		item.pszText = tmppath;
		item.cchTextMax = MAX_KEY_LENGTH;
		item.hItem = hItem;
		TreeView_GetItem(hTreeView, &item);

		wcscpy(tmpoverlap, buf);
		wsprintf(buf, L"%s%s%s", item.pszText, L"\\", tmpoverlap);
	}
}

TCHAR* removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH])
{
	TCHAR *hkeyStr = new TCHAR[MAX_KEY_LENGTH];
	if (wcsstr(fullPath, _T("HKEY_CLASSES_ROOT")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CLASSES_ROOT\\"));
		wcscpy(hkeyStr, _T("HKEY_CLASSES_ROOT\\"));
		return hkeyStr;
	} else
	if (wcsstr(fullPath, _T("HKEY_CURRENT_USER")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CURRENT_USER\\"));
		wcscpy(hkeyStr, _T("HKEY_CURRENT_USER\\"));
		return hkeyStr;
	} else
	if (wcsstr(fullPath, _T("HKEY_LOCAL_MACHINE")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_LOCAL_MACHINE\\"));
		wcscpy(hkeyStr, _T("HKEY_LOCAL_MACHINE\\"));
		return hkeyStr;
	} else
	if (wcsstr(fullPath, _T("HKEY_USERS")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_USERS\\"));
		wcscpy(hkeyStr, _T("HKEY_USERS\\"));
		return hkeyStr;
	} else
	if (wcsstr(fullPath, _T("HKEY_CURRENT_CONFIG")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CURRENT_CONFIG\\"));
		wcscpy(hkeyStr, _T("HKEY_CURRENT_CONFIG\\"));
		return hkeyStr;
	} else	return _T("");
}

//функция удаления подстроки substr из строки str
void removeSubStr(TCHAR str[MAX_KEY_LENGTH], TCHAR substr[MAX_KEY_LENGTH])
{
	int pos = wcsstr(str, substr) - str;
	if (pos >= 0)
	{
		wcscpy(str + pos, str + pos + wcslen(substr));
	}
}

void ClearSubBranches(HWND hTreeView, HTREEITEM hItem)
{
	//Функция проверяет есть ли текущей ветки ребенок
	//Если ребенок есть, то получаем его хэндл  и вызываем эту функцию снова
	//до тех пор пока найдем ребенка, у которого нет своих детей
	//и убиваем его.
	HTREEITEM hChild = TreeView_GetChild(hTreeView, hItem);
	if (hChild)
		ClearSubBranches(hTreeView, hChild);

	//Также рекурсивно проверяем наличие сиблингов у ветки
	//сиблинги удаляются после удаления всех детей с конца списка
	HTREEITEM hSibling = TreeView_GetNextSibling(hTreeView, hItem);
	if (hSibling)
		ClearSubBranches(hTreeView, hSibling);

	TreeView_DeleteItem(hTreeView, hItem);

}
//функция удаления ветки hItem из дерева
void clearBranch(HWND hTreeView, HTREEITEM hItem)
{
	HTREEITEM hChild;
	if (hChild = TreeView_GetChild(hTreeView, hItem))
	{
		//вызов рекурсивной функции удаления из дерева
		ClearSubBranches(hTreeView, hChild);
	}
}

TV_ITEM getCurrentItem(HWND hTreeView, HTREEITEM hItem)
{
	TV_ITEM item = { 0 };
	TCHAR tmppath[MAX_KEY_LENGTH];
	TCHAR tmpoverlap[MAX_KEY_LENGTH];
	HTREEITEM tmphItem;

	item.mask = TVIF_TEXT;
	item.pszText = tmppath;
	item.cchTextMax = MAX_KEY_LENGTH;
	item.hItem = hItem;
	TreeView_GetItem(hTreeView, &item);
	return item;
}

int regValueType(TCHAR type[TYPE_LENGTH])
{
	if (wcsstr(type, _T("REG_SZ")) != NULL)
	{
		return REG_SZ;
	}
	if (wcsstr(type, _T("REG_DWORD")) != NULL)
	{
		return REG_DWORD;
	}
	if (wcsstr(type, _T("REG_BINARY")) != NULL)
	{
		return REG_BINARY;
	}

	return REG_SZ;
}

unsigned int parseTcharToInt(TCHAR value[MAX_KEY_LENGTH])
{
	if (wcslen(value) == 0)	return -1;

	TCHAR *nSub = wcsstr(value, L"(");
	int nSubInt = nSub - value; //ищем значение в круглых скобах и переводим его в int
	TCHAR numberInt[20];
	int i = 0;
	while (value[i + nSubInt+1] != L')')
	{
		numberInt[i] = value[i + nSubInt+1];
		i++;
	}
	numberInt[i] = L'\0';
	return _wtoi(numberInt);
}

void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH])
{
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = SendMessage(hlistView, LVM_GETITEMCOUNT, 0, 0);
	lvitem.iSubItem = 0;
	if (wcslen(name) == 0)
		lvitem.pszText = L"(По умолчанию)";
	else
		lvitem.pszText = name;
	int iActualItem = SendMessage(hlistView, LVM_INSERTITEM, 0, (WPARAM)&lvitem);

	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 1;
	lvitem.pszText = type;
	SendMessage(hlistView, LVM_SETITEM, 0, (WPARAM)&lvitem);

	lvitem.iSubItem = 2;
	lvitem.pszText = value;
	SendMessage(hlistView, LVM_SETITEM, 0, (WPARAM)&lvitem);
}

void initSearch(SearchResults *searchResults)
{
	TCHAR *d;
	//Производим очистку стэков если это необходимо
	while (!searchResults->searchStack.empty())
	{
		d = searchResults->searchStack.top();
		delete d;
		searchResults->searchStack.pop();
	}

	while (!searchResults->path.empty())
	{
		d = searchResults->path.top();
		delete d;
		searchResults->path.pop();
	}
	//инициализируем стэк поиска, создаем корневые элементы начиная с конца
	searchResults->count.push(5);//5 корневых веток реестра
	/*
	TCHAR *hkcc = new TCHAR[30];
	wcscpy(hkcc, L"HKEY_CURRENT_CONFIG");
	searchResults->searchStack.push(hkcc);

	TCHAR *hku = new TCHAR[30];
	wcscpy(hku, L"HKEY_USERS");
	searchResults->searchStack.push(hku);
	

	TCHAR *hkcm = new TCHAR[30];
	wcscpy(hkcm, L"HKEY_LOCAL_MACHINE");
	searchResults->searchStack.push(hkcm);
	
	*/
	TCHAR *hkcu = new TCHAR[30];
	wcscpy(hkcu, L"HKEY_CURRENT_USER");
	searchResults->searchStack.push(hkcu);
	/*
	
	TCHAR *hkcr = new TCHAR[30];
	wcscpy(hkcr, L"HKEY_CLASSES_ROOT");
	searchResults->searchStack.push(hkcr);
	*/
}

HTREEITEM insertInTreeView(HWND hWnd, HTREEITEM parent, TCHAR achKey[MAX_KEY_LENGTH])
{
	//Создание элемента в tree view
	TV_INSERTSTRUCT tvinsert;
	//parent.pszText;
	tvinsert.hParent = parent;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT;// | TVIF_CHILDREN;
	tvinsert.item.pszText = achKey;
	return (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

//функция возвращает указатель на корневой каталог по полученному пути
//если строка не является валидным ключем реестра, то возвращается 0
HKEY determineHKEY(TCHAR path[MAX_KEY_LENGTH])
{
	if (wcsstr(path, _T("HKEY_CLASSES_ROOT")) != NULL)
	{
		return HKCR;
	}
	if (wcsstr(path, _T("HKEY_CURRENT_USER")) != NULL)
	{
		return HKCU;
	}
	if (wcsstr(path, _T("HKEY_LOCAL_MACHINE")) != NULL)
	{
		return HKLM;
	}
	if (wcsstr(path, _T("HKEY_USERS")) != NULL)
	{
		return HKU;
	}
	if (wcsstr(path, _T("HKEY_CURRENT_CONFIG")) != NULL)
	{
		return HKCC;
	}
	else
		return NULL;
}

bool loadRegistry(HWND hWnd, HWND regeditTreeView, HTREEITEM *MyPC)
{

	TV_INSERTSTRUCT tvinsert;
	HTREEITEM hkeyItem;

	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_ROOT;
	tvinsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	tvinsert.item.pszText = ROOT_ITEM_NAME;
	*MyPC = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = *MyPC;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.pszText = L"HKEY_CLASSES_ROOT";
	hkeyItem = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.item.pszText = L"HKEY_CURRENT_USER";
	hkeyItem = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.item.pszText = L"HKEY_LOCAL_MACHINE";
	hkeyItem = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.item.pszText = L"HKEY_USERS";
	hkeyItem = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.item.pszText = L"HKEY_CURRENT_CONFIG";
	hkeyItem = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	TreeView_Expand(regeditTreeView, *MyPC, TVE_EXPAND);

	return true;
}