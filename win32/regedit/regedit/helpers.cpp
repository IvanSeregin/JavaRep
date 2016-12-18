#include "definitions.h"

void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf)
{
	TV_ITEM item = { 0 };
	WCHAR tmppath[MAX_KEY_LENGTH];
	WCHAR tmpoverlap[MAX_KEY_LENGTH];
	HTREEITEM tmphItem;

	item.mask = TVIF_TEXT;
	item.pszText = tmppath;
	item.cchTextMax = MAX_KEY_LENGTH;
	item.hItem = hItem;
	TreeView_GetItem(hTreeView, &item);

	//wsprintf(buf, L"%s%s", item.pszText, L"\\");

	while ((hItem = TreeView_GetParent(hTreeView, hItem)) != *root)
	{
		item.mask = TVIF_TEXT;
		item.pszText = tmppath;
		item.cchTextMax = MAX_KEY_LENGTH;
		item.hItem = hItem;
		TreeView_GetItem(hTreeView, &item);

		//wcscpy(tmpoverlap, buf);
		//wsprintf(buf, L"%s%s%s", item.pszText, L"\\", tmpoverlap);
	}
	//wcscat(buf, L"*");
}