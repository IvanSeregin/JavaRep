#include "definitions.h"
#include <stdio.h>
#include <string>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

HKEY dumpKey(HKEY hKey, TCHAR path[]);

bool dumpRegistry()
{
	HKEY key;
	TCHAR  path[] = _T("AppEvents");

	if (RegOpenKeyEx(HKEY_CURRENT_USER,	path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpKey(key, path);
		RegCloseKey(key);
	}

	return true;
}

HKEY dumpKey(HKEY hKey, TCHAR path[])
{

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	 // Enumerate the subkeys, until RegEnumKeyEx fails.
	if (cSubKeys)
	{
		for (i = 0; i<cSubKeys; i++)
		{
			TCHAR tmpPath[] = _T("");
			wcscat(tmpPath, path);
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);

				wcscat(tmpPath, _T("\\"));
				wcscat(tmpPath, achKey);

				HKEY tmpKey = hKey;
				if (RegOpenKeyEx(HKEY_CURRENT_USER, tmpPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS)
				{
					dumpKey(tmpKey, tmpPath);
					RegCloseKey(tmpKey);
				}
			}
		}
	}
	
	return NULL;
}

bool loadRegistry(HWND hWnd, HWND regeditTreeView)
{
	/*
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM Parent;
	HTREEITEM Before;
	HTREEITEM Root;

	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_ROOT;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = L"Мой компьютер";
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = Parent;          // create a child to our parent ;-)
	tvinsert.hInsertAfter = TVI_LAST;   // put it under [ last item ]
	tvinsert.item.pszText = L"Child 1";  // name it whatever u want
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	Root = Parent;
	Before = Parent;       // handle of the before root
	tvinsert.hParent = Parent; // handle of the above data
	tvinsert.hInsertAfter = TVI_LAST;  // below parent
	tvinsert.item.pszText = L"Child 1";
	// draw it, and save the parent for more child making.
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = Parent;
	tvinsert.item.pszText = L"Child of Child1";
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = Parent;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.pszText = L"Double Click Me!";
	tvinsert.item.mask = TVIF_TEXT;
	SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = Before;         // handle of the above data
	tvinsert.hInsertAfter = TVI_LAST;  // below parent
	tvinsert.item.pszText = L"Child 2";
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = NULL; // top most level no need handle
	tvinsert.hInsertAfter = TVI_LAST; // work as root level
	tvinsert.item.pszText = L"Parent2";
	Parent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
	*/
	return true;
}


