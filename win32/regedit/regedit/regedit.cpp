#include "definitions.h"
#include <stdio.h>
#include <string>



HKEY dumpCatalog(HKEY hRootKey, HKEY hKey, TCHAR path[], HANDLE hFile);
HKEY determineHKEY(TCHAR path[MAX_KEY_LENGTH]);
//HKEY loadKey(HWND hWnd, HKEY hRootKey, HKEY hKey, TCHAR path[], HTREEITEM Parent);

bool dumpRegistry()
{
	HKEY key;
	TCHAR fileName[MAX_KEY_LENGTH] = _T("");
	PWSTR fName;
	//Получаем путь до рабочего стола текущего пользователя
	//и генерируем имя файла для сохранения реестра
	if (SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &fName) == S_OK)
	{
		wcscpy(fileName, fName);
		wcscat_s(fileName, _T("\\regedit.txt"));
	}
	else
		return false;
	
	//Пробуем открыть полученный файл
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//wfstream out("events.txt", wios::out);
	
	if (!hFile) return false;
	
	//Если файл был открыт удачно, открываем на чтение первую ветку реестра 
	//и вызываем функцию сохранения реестра dumpKey(). 
	//Данная функция является рекурсивной!!!
	TCHAR  path[MAX_KEY_LENGTH] = _T("");
	WriteFile(hFile, "[HKEY_CLASSES_ROOT]\n", strlen("[HKEY_CLASSES_ROOT]\n"), NULL, NULL);
	if (RegOpenKeyEx(HKCR,	path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_CURRENT_USER]\n", strlen("[HKEY_CURRENT_USER]\n"), NULL, NULL);
	if (RegOpenKeyEx(HKCU, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_LOCAL_MACHINE]\n", strlen("[HKEY_LOCAL_MACHINE]\n"), NULL, NULL);
	if (RegOpenKeyEx(HKLM, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_USERS]\n", strlen("[HKEY_USERS]\n"), NULL, NULL);
	if (RegOpenKeyEx(HKU, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_CURRENT_CONFIG]\n", strlen("[HKEY_CURRENT_CONFIG]\n"), NULL, NULL);
	if (RegOpenKeyEx(HKCC, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}

	CloseHandle(hFile);
	return true;
}

HKEY dumpCatalog(HKEY hRootKey, HKEY hKey, TCHAR path[], HANDLE hFile)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // буфер для подкаталогов
	DWORD    cbName;                  
	TCHAR    achClass[MAX_PATH] = TEXT("");  
	DWORD    cchClassName = MAX_PATH;  
	DWORD    cSubKeys = 0;         
	DWORD    cbMaxSubKey;          
	DWORD    cchMaxClass;         
	DWORD    cValues;             
	DWORD    cchMaxValue;         
	DWORD    cbMaxValueData;      
	DWORD    cbSecurityDescriptor;
	FILETIME ftLastWriteTime;     

	DWORD i, retCode;

	DWORD cchValue = MAX_VALUE_NAME;

	// Получаем информацию о текущей ветке
	retCode = RegQueryInfoKey(hKey, achClass, &cchClassName, NULL,&cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);       

	 // cSubKeys - количество подветок текущей ветки
	if (cSubKeys)
	{
		//---------------------------------------------ПОЛУЧЕНИЕ НАЗВАНИЯ ВЕТКИ РЕЕСТРА ------------------
		//Перебираем все подветки
 		for (i = 0; i<cSubKeys; i++)
		{
			TCHAR tmpPath[MAX_KEY_LENGTH] = _T(""); //для временного сохранения пути до подветки
			wcscat_s(tmpPath, path); //запоминаем текущую подветку
			cbName = MAX_KEY_LENGTH;
			//Получаем список подветок текущей ветки, которая указана в hKey
			retCode = RegEnumKeyEx(hKey, i,	achKey,	&cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//Если информация получена, то производим перебор все подветок
			if (retCode == ERROR_SUCCESS)
			{

				if (wcslen(path)) wcscat_s(tmpPath, _T("\\"));
				wcscat_s(tmpPath, achKey);

				HKEY tmpKey = hKey;
				//Открываем текущий подкаталог
				if (RegOpenKeyEx(hRootKey, tmpPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS)
				{
					//Если все ок, то записываем о нем информацию в файл.
					char *ch = new char[MAX_KEY_LENGTH];
					wcstombs(ch, tmpPath, MAX_KEY_LENGTH);

					WriteFile(hFile, "[", strlen("["), NULL, NULL);
					WriteFile(hFile, ch, strlen(ch), NULL, NULL);
					WriteFile(hFile, "]\n", strlen("]\n"), NULL, NULL);
					
					//---------------------------------------ПОЛУЧЕНИЕ КЛЮЧЕЙ В ТЕКУЩЕЙ ВЕТКЕ
					//Далее читаем все ключи и их значения из текущей ветки реесра
					LONG lResult;
					DWORD j = 0;
					do
					{
						TCHAR Name[MAX_KEY_LENGTH]; //имя ключа
						TCHAR Value[MAX_KEY_LENGTH]; //значение ключа
						DWORD cName = 4096;
						DWORD cValue = 4096;
						DWORD type =  0;

						lResult = RegEnumValue(tmpKey, j, Name, &(cName = MAX_KEY_LENGTH), NULL, &type, (PBYTE)Value, &(cValue = MAX_KEY_LENGTH));
						if (lResult == ERROR_SUCCESS)
						{
							//если есть ключ - пишем его в файл
							char *vl = new char[MAX_KEY_LENGTH];
							//memset(vl, 0x0, sizeof(vl));
							if (type == REG_DWORD) // значение ключа типа REG_DWORD
							{							
								_ultoa(*Value, vl, 16);
							}
							else if (type == REG_BINARY) //значение ключа типа REG_BINARY
							{
								ultoa(*Value, vl, 16);
							}
							else //значение ключа текстовое
							{
								wcstombs(vl, Value, MAX_KEY_LENGTH);
							}

							//Конвертируем TCHAR в char для вывода в файл
							char *nm = new char[MAX_KEY_LENGTH];
							wcstombs(nm, Name, MAX_KEY_LENGTH);

							//Записываем имя параметра в файл
							if (strlen(nm) == 0)
								WriteFile(hFile, "@", strlen("@"), NULL, NULL);
							else
								WriteFile(hFile, nm, strlen(nm), NULL, NULL);

							//Записываем значение параметра в файл
							WriteFile(hFile, "=", strlen("="), NULL, NULL);
							WriteFile(hFile, vl, strlen(vl), NULL, NULL);
							WriteFile(hFile, "\n", strlen("\n"), NULL, NULL);

							//Прибираемся в памяти
							delete[] nm;
							delete[] vl;
						}
						j++; //Переходим к следующему параметру
					} while (lResult != ERROR_NO_MORE_ITEMS); //Если параметров больше нет - выходим
					///---------------------------------------ПОЛУЧЕНИЕ КЛЮЧЕЙ В ТЕКУЩЕЙ ВЕТКЕ ЗАКОНЧЕНО
					//Прибираемся в памяти
					delete[] ch;
					//Вызываем текущую функцию повторно, но уже для поиска подкаталогов текущего каталого
					dumpCatalog(hRootKey, tmpKey, tmpPath, hFile);
					RegCloseKey(tmpKey);
					WriteFile(hFile, "\n", strlen("\n"), NULL, NULL);
				}
			}
		}
	}
	
	return NULL;
}

bool loadRegistry(HWND hWnd, HWND regeditTreeView, HTREEITEM *MyPC)
{
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM hkeyItem;

	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_ROOT;
	tvinsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	tvinsert.item.pszText = L"Мой компьютер";
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

void updateSubCatalogs(HWND hTreeView, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]) //сканирует и добавляет каталоги к выбранному каталогу
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // буфер для подкаталогов
	DWORD    cbName;
	TCHAR    achClass[MAX_PATH] = TEXT("");
	DWORD    cchClassName = MAX_PATH;
	DWORD    cSubKeys = 0;
	DWORD    cbMaxSubKey;
	DWORD    cchMaxClass;
	DWORD    cValues;
	DWORD    cchMaxValue;
	DWORD    cbMaxValueData;
	DWORD    cbSecurityDescriptor;
	FILETIME ftLastWriteTime;

	DWORD i, retCode;
	DWORD cchValue = MAX_VALUE_NAME;
	HKEY key;

	HKEY hRootKey = determineHKEY(fullPath);
	if (hRootKey == NULL) return;

	
	if (RegOpenKeyEx(hRootKey, fullPath, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{

	}
	/**/
	// Получаем информацию о текущей ветке
	retCode = RegQueryInfoKey(hKey, achClass, &cchClassName, NULL, &cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);

	// cSubKeys - количество подветок текущей ветки
	if (cSubKeys)
	{
		//---------------------------------------------ПОЛУЧЕНИЕ НАЗВАНИЯ ВЕТКИ РЕЕСТРА ------------------
		//Перебираем все подветки
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			//Получаем список подветок текущей ветки, которая указана в hKey
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//Если информация получена, то производим перебор все подветок
			if (retCode == ERROR_SUCCESS)
			{
				HKEY tmpKey = hKey;
				//Открываем текущий подкаталог
				if (RegOpenKeyEx(hRootKey, tmpPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS)
				{
					//Создание элемента в tree view
					TV_INSERTSTRUCT tvinsert;
					HTREEITEM tmpParent = Parent;
					tvinsert.hParent = tmpParent;
					tvinsert.hInsertAfter = TVI_LAST;
					tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tvinsert.item.pszText = achKey;
					tmpParent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
					//Вызываем текущую функцию повторно, но уже для поиска подкаталогов текущего каталого
					RegCloseKey(tmpKey);
				}
			}
		}
	}
	*/
}

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

/*
HKEY loadKey(HWND hWnd, HKEY hRootKey, HKEY hKey, TCHAR path[], HTREEITEM Parent)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // буфер для подкаталогов
	DWORD    cbName;
	TCHAR    achClass[MAX_PATH] = TEXT("");
	DWORD    cchClassName = MAX_PATH;
	DWORD    cSubKeys = 0;
	DWORD    cbMaxSubKey;
	DWORD    cchMaxClass;
	DWORD    cValues;
	DWORD    cchMaxValue;
	DWORD    cbMaxValueData;
	DWORD    cbSecurityDescriptor;
	FILETIME ftLastWriteTime;

	DWORD i, retCode;

	DWORD cchValue = MAX_VALUE_NAME;

	// Получаем информацию о текущей ветке
	retCode = RegQueryInfoKey(hKey, achClass, &cchClassName, NULL, &cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);

	// cSubKeys - количество подветок текущей ветки
	if (cSubKeys)
	{
		//---------------------------------------------ПОЛУЧЕНИЕ НАЗВАНИЯ ВЕТКИ РЕЕСТРА ------------------
		//Перебираем все подветки
		for (i = 0; i < cSubKeys; i++)
		{
			TCHAR tmpPath[MAX_KEY_LENGTH] = _T(""); //для временного сохранения пути до подветки
			wcscat_s(tmpPath, path); //запоминаем текущую подветку
			cbName = MAX_KEY_LENGTH;
			//Получаем список подветок текущей ветки, которая указана в hKey
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//Если информация получена, то производим перебор все подветок
			if (retCode == ERROR_SUCCESS)
			{

				if (wcslen(path)) wcscat_s(tmpPath, _T("\\"));
				wcscat_s(tmpPath, achKey);

				HKEY tmpKey = hKey;
				//Открываем текущий подкаталог
				if (RegOpenKeyEx(hRootKey, tmpPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS)
				{
					//Создание элемента в tree view
					TV_INSERTSTRUCT tvinsert;
					HTREEITEM tmpParent = Parent;
					tvinsert.hParent = tmpParent;
					tvinsert.hInsertAfter = TVI_LAST;
					tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tvinsert.item.pszText = achKey;
					tmpParent = (HTREEITEM)SendDlgItemMessage(hWnd, REG_TREE_VIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
					//Вызываем текущую функцию повторно, но уже для поиска подкаталогов текущего каталого
					loadKey(hWnd, hRootKey, tmpKey, tmpPath, tmpParent);
					RegCloseKey(tmpKey);
				}
			}
		}
	}
	return NULL;
}
*/