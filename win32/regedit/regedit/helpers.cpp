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

void removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH])
{
	if (wcsstr(fullPath, _T("HKEY_CLASSES_ROOT")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CLASSES_ROOT\\"));
	} else
	if (wcsstr(fullPath, _T("HKEY_CURRENT_USER")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CURRENT_USER\\"));
	} else
	if (wcsstr(fullPath, _T("HKEY_LOCAL_MACHINE")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_LOCAL_MACHINE\\"));
	} else
	if (wcsstr(fullPath, _T("HKEY_USERS")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_USERS\\"));
	} else
	if (wcsstr(fullPath, _T("HKEY_CURRENT_CONFIG")) != NULL)
	{
		removeSubStr(fullPath, _T("HKEY_CURRENT_CONFIG\\"));
	} else	return;
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