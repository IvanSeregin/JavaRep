#include "definitions.h"
void removeSubStr(TCHAR str[MAX_KEY_LENGTH], TCHAR substr[MAX_KEY_LENGTH]);
void ClearSubBranches(HWND hTreeView, HTREEITEM hItem);
//������� ��������� ������� ���� �� �������� �������, ������� �����������
//� TreeView ��� hItem
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

//������� �������� ��������� substr �� ������ str
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
	//������� ��������� ���� �� ������� ����� �������
	//���� ������� ����, �� �������� ��� �����  � �������� ��� ������� �����
	//�� ��� ��� ���� ������ �������, � �������� ��� ����� �����
	//� ������� ���.
	HTREEITEM hChild = TreeView_GetChild(hTreeView, hItem);
	if (hChild)
		ClearSubBranches(hTreeView, hChild);

	//����� ���������� ��������� ������� ��������� � �����
	//�������� ��������� ����� �������� ���� ����� � ����� ������
	HTREEITEM hSibling = TreeView_GetNextSibling(hTreeView, hItem);
	if (hSibling)
		ClearSubBranches(hTreeView, hSibling);

	TreeView_DeleteItem(hTreeView, hItem);

}
//������� �������� ����� hItem �� ������
void clearBranch(HWND hTreeView, HTREEITEM hItem)
{
	HTREEITEM hChild;
	if (hChild = TreeView_GetChild(hTreeView, hItem))
	{
		//����� ����������� ������� �������� �� ������
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
	int nSubInt = nSub - value; //���� �������� � ������� ������ � ��������� ��� � int
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
