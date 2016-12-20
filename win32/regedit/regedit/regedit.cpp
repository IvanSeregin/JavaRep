#include "definitions.h"
#include <stdio.h>
#include <string>



HKEY dumpCatalog(HKEY hRootKey, HKEY hKey, TCHAR path[], HANDLE hFile); //сохранение указанного каталога в файл
HKEY determineHKEY(TCHAR path[MAX_KEY_LENGTH]); // определяет ветку реестра по полному пути до каталога
HTREEITEM insertInTreeView(HWND hWnd, HTREEITEM parent, TCHAR achKey[MAX_KEY_LENGTH]); //вставляет новый подкаталог в текущий каталог
void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH]); //вставляет новую строку в ListView
HKEY openKey(TCHAR fullPath[MAX_KEY_LENGTH]);

//функция сохраняет весь реестр в файл
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
								//_ultoa(*Value, vl, 16);
								DWORD *pdwd = new DWORD;
								DWORD type = REG_DWORD;//тип параметра - DWORD
								DWORD datalen = sizeof(pdwd);
								RegQueryValueEx(tmpKey, Name, 0, &type, (BYTE*)(pdwd), &datalen);
								wsprintf(Value, L"0x%08x (%d) ", *pdwd, *pdwd);
								wcstombs(vl, Value, MAX_KEY_LENGTH);
								//insertRow(hListView, Name, _T("REG_DWORD"), Value);
								delete pdwd;
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

void updateSubCatalogs(HWND hWnd, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]) //сканирует и добавляет каталоги к выбранному каталогу
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

	removeHKRoot(fullPath);

	if (RegOpenKeyEx(hRootKey, fullPath, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		retCode = RegQueryInfoKey(key, achClass, &cchClassName, NULL, &cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			//Получаем список подветок текущей ветки, которая указана в hKey
			retCode = RegEnumKeyEx(key, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//Если информация получена, то производим перебор все подветок
			
			HTREEITEM Parent2lvl; //родитель для второго уровня узлов дерева
			HKEY key2lvl; //ключи второго уровня
			TCHAR fullPath2lvl[MAX_KEY_LENGTH]; //путь до каталогов второго уровня
			DWORD cSubKey2lvl; //количество подключей второго уровня

			if (retCode == ERROR_SUCCESS)
			{
				Parent2lvl = insertInTreeView(hWnd, Parent.hItem, achKey);
			}
			
			//вставляем каталоги второго уровня для текущего каталога
			//но только в том случае если буфер для хранения нового пути больше
			//чем длина имени текущего каталога  + длина имени вставляемого каталога
			
			if ((wcslen(fullPath) + wcslen(achKey)) < MAX_KEY_LENGTH)
			{
				wcscpy_s(fullPath2lvl, fullPath);
				wcscat_s(fullPath2lvl, achKey);
				
				if (RegOpenKeyEx(hRootKey, fullPath2lvl, 0, KEY_READ, &key2lvl) == ERROR_SUCCESS)
				{
					retCode = RegQueryInfoKey(key2lvl, achClass, &cchClassName, NULL, &cSubKey2lvl, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);
					for (int j = 0; j < cSubKey2lvl; j++)
					{
						cbName = MAX_KEY_LENGTH;
						//Получаем список подветок текущей ветки, которая указана в hKey
						retCode = RegEnumKeyEx(key2lvl, j, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
						//Вносим полученную ветку в качестве подкаталога
						if (retCode == ERROR_SUCCESS)
						{
							insertInTreeView(hWnd, Parent2lvl, achKey);
						}
					
					}
				}
			}
			
			
		}
		RegCloseKey(key);
	}
	else
	{
		return;
	}
}
//функция возвращает корневой каталог по полученному пути
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

void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]) //выводит список ключей и их параметров
{
	ListView_DeleteAllItems(hListView);
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
	DWORD lResult;
	DWORD retCode;

	DWORD cchValue = MAX_VALUE_NAME;

	HKEY key, hKey = determineHKEY(fullPath);
	if (hKey == NULL) return;
	removeHKRoot(fullPath);

	if (RegOpenKeyEx(hKey, fullPath, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		DWORD j = 0;
		do
		{
			TCHAR Name[MAX_KEY_LENGTH]; //имя ключа
			TCHAR Value[MAX_KEY_LENGTH]; //значение ключа
			DWORD cName = 4096;
			DWORD cValue = 4096;
			DWORD type = 0;

			lResult = RegEnumValue(key, j, Name, &(cName = MAX_KEY_LENGTH), NULL, &type, (PBYTE)Value, &(cValue = MAX_KEY_LENGTH));
			if (lResult == ERROR_SUCCESS)
			{
				if (type == REG_DWORD) // значение ключа типа REG_DWORD
				{
					DWORD *pdwd = new DWORD;
					DWORD type = REG_DWORD;//тип параметра - DWORD
					DWORD datalen = sizeof(pdwd);
					RegQueryValueEx(key, Name, 0, &type, (BYTE*)(pdwd), &datalen);
					wsprintf(Value, L"0x%08x (%d) ", *pdwd, *pdwd);
					insertRow(hListView, Name, _T("REG_DWORD"), Value);
					delete pdwd;

				}
				else if (type == REG_BINARY) //значение ключа типа REG_BINARY
				{
					insertRow(hListView, Name, _T("REG_BINARY"), Value);
				}
				else //значение ключа текстовое
				{
					insertRow(hListView, Name, _T("REG_SZ"), Value);
				}
			}
			j++; //Переходим к следующему параметру
		} while (lResult != ERROR_NO_MORE_ITEMS); //Если параметров больше нет - выходим

	}

	
}

int deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH])
{
	HKEY key = openKey(fullPath);
	if (!RegDeleteValue(key, keyName)) return NULL;
	return 0;
}

HKEY openKey(TCHAR fullPath[MAX_KEY_LENGTH])
{
	HKEY key, hKey = determineHKEY(fullPath);
	removeHKRoot(fullPath);
	if (RegOpenKeyEx(hKey, fullPath, 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS)
	{
		return key;
	}

	return NULL;
}

int deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH])
{
	HKEY hKey = determineHKEY(fullPath);
	removeHKRoot(fullPath);
	return RegDeleteTree(hKey, fullPath);
}
