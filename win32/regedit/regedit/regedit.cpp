#include "definitions.h" 
#include <stdio.h>
#include <string>

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
	DWORD written = 0;
	WriteFile(hFile, "[HKEY_CLASSES_ROOT]\n", strlen("[HKEY_CLASSES_ROOT]\n"), &written, NULL);
	if (RegOpenKeyEx(HKCR,	path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_CURRENT_USER]\n", strlen("[HKEY_CURRENT_USER]\n"), &written, NULL);
	if (RegOpenKeyEx(HKCU, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_LOCAL_MACHINE]\n", strlen("[HKEY_LOCAL_MACHINE]\n"), &written, NULL);
	if (RegOpenKeyEx(HKLM, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_USERS]\n", strlen("[HKEY_USERS]\n"), &written, NULL);
	if (RegOpenKeyEx(HKU, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}
	WriteFile(hFile, "[HKEY_CURRENT_CONFIG]\n", strlen("[HKEY_CURRENT_CONFIG]\n"), &written, NULL);
	if (RegOpenKeyEx(HKCC, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(key, key, path, hFile);
		RegCloseKey(key);
	}

	CloseHandle(hFile);
	return true;
}

bool dumBranch(TCHAR path[MAX_KEY_LENGTH])
{
	HKEY key;
	TCHAR fileName[MAX_KEY_LENGTH] = _T("");
	TCHAR *hkeyStr;
	char nm[MAX_KEY_LENGTH] = "";
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

	if (!hFile) return false;
	//получаем указатель на корневой каталог
	HKEY hRootKey = determineHKEY(path);
	//получаем имя корневого каталога в текстовом виде
	hkeyStr = removeHKRoot(path);
	//записываем имя корневого каталога в файл
	wcstombs(nm, hkeyStr, MAX_KEY_LENGTH);
	DWORD written = 0;
	WriteFile(hFile, nm, strlen(nm), &written, NULL);
	WriteFile(hFile, "\n", strlen("\n"), &written, NULL);
	delete[] hkeyStr;
	//сохраняем ветку реестра в файл
	if (RegOpenKeyEx(hRootKey, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		dumpCatalog(hRootKey, key, path, hFile);
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
	DWORD written = 0;

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

					WriteFile(hFile, "[", strlen("["), &written, NULL);
					WriteFile(hFile, ch, strlen(ch), &written, NULL);
					WriteFile(hFile, "]\n", strlen("]\n"), &written, NULL);
					
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
								WriteFile(hFile, "@", strlen("@"), &written, NULL);
							else
								WriteFile(hFile, nm, strlen(nm), &written, NULL);

							//Записываем значение параметра в файл
							WriteFile(hFile, "=", strlen("="), &written, NULL);
							WriteFile(hFile, vl, strlen(vl), &written, NULL);
							WriteFile(hFile, "\n", strlen("\n"), &written, NULL);

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
					WriteFile(hFile, "\n", strlen("\n"), &written, NULL);
				}
			}
		}
	}
	
	return NULL;
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

void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]) //выводит список параметров и их значений
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
					//wsprintf(Value, L"0x%08x", *pdwd);
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

LRESULT deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH])
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

LRESULT deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH])
{
	HKEY hKey = determineHKEY(fullPath);
	removeHKRoot(fullPath);
	return RegDeleteTree(hKey, fullPath);
}

LRESULT renameBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH])
{
	HKEY hKey = determineHKEY(fullPath);
	removeHKRoot(fullPath);
	return RegRenameKey(hKey, fullPath, newName);
}

LRESULT renameParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR oldName[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH], TCHAR Value[MAX_KEY_LENGTH], TCHAR type[TYPE_LENGTH])
{
	//сохраняем данные параметра
	HKEY hKey = determineHKEY(fullPath);
	HKEY key;
	HKEY keyToDel = openKey(fullPath);

	//Удаляем старый параметр
	RegDeleteValue(keyToDel, oldName);

	//создаем новый параметр newValue со значемнием value
	removeHKRoot(fullPath);
	//Открываем нужную ветку с сооветствующими правами
	LRESULT result = RegOpenKeyEx(hKey, fullPath, 0, KEY_SET_VALUE, &key);
	//Определяем тип параметра
	int typeInt = regValueType(type);
	if (typeInt == REG_SZ)
	{
		//записываем текстовый параметр
		result = SHSetValue(hKey, fullPath, newName, typeInt, Value, wcslen(Value) * 2);
	}
	else if (typeInt == REG_DWORD)
	{
		//Записываем параметр типа dword
		//парсим текст, берем из него значение в скобках и преобразуем в число
		unsigned int v = parseTcharToInt(Value);
		result = SHSetValue(hKey, fullPath, newName, typeInt, (BYTE*)&v, 4);
	}
	else if (typeInt == REG_BINARY)
	{

	}
	return result;
}

LRESULT addBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newBranchName[MAX_KEY_LENGTH])
{
	HKEY hKey = determineHKEY(fullPath);
	removeHKRoot(fullPath);
	HKEY key;
	LRESULT result = RegOpenKeyEx(hKey, fullPath, 0, KEY_CREATE_SUB_KEY, &key);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKey(key, newBranchName, &hKey);
	}
	return result;
}

void search(TCHAR str[MAX_KEY_LENGTH], SearchResults *searchResults)
{
	
}

BOOL searchInReg(TCHAR str[MAX_KEY_LENGTH], SearchResults *searchResults, std::stack <TCHAR*> *tmpPath)
{
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);

	while (!searchResults->searchStack.empty())
	{
		TCHAR    achKey[MAX_KEY_LENGTH];   // буфер для подкаталогов
		DWORD    cbName;
		TCHAR    achClass[MAX_KEY_LENGTH] = L"";
		DWORD    cchClassName = MAX_KEY_LENGTH;
		DWORD    cSubKeys = 0;
		DWORD    cbMaxSubKey;
		DWORD    cchMaxClass;
		DWORD    cValues;
		DWORD    cchMaxValue;
		DWORD    cbMaxValueData;
		DWORD    cbSecurityDescriptor;
		FILETIME ftLastWriteTime;

		DWORD retCode;
		TCHAR fullPath[MAX_KEY_LENGTH] = L"";
		TCHAR *catalog;

		//получаем следующий каталог для просмотра
		catalog = searchResults->searchStack.top();
		searchResults->searchStack.pop();
		//ложим его в стэк пути
		searchResults->path.push(catalog);
		//получаем полный путь до текущего каталога
		reconstructFullPath(*searchResults, fullPath);
		//получаем указатель на корневой элемент реестра
		HKEY hKey = determineHKEY(fullPath);
		//удаляем из пути имя корневого элемента реестра
		TCHAR *tmp = removeHKRoot(fullPath);
		delete[] tmp;
		HKEY tmpKey;
		//открываем текущую ветку
		if (RegOpenKeyEx(hKey, fullPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS);
		{
			//проверяем текущую ветку на предмет наличия в ней искомой подстроки 
			if (wcsstr(fullPath, str) != NULL)
			{
				*tmpPath = searchResults->path;
				changeCounts(searchResults);
				return SUBSTR_FOUND;
			}
			//проверяем параметры текущей ветки на предмет наличия в них искомой подстроки
			int j = 0;
			do
			{
				TCHAR Name[MAX_KEY_LENGTH]; //имя ключа
				TCHAR Value[MAX_KEY_LENGTH]; //значение ключа
				DWORD cName = 4096;
				DWORD cValue = 4096;
				DWORD type = 0;
				retCode = RegEnumValue(tmpKey, j, Name, &(cName = MAX_KEY_LENGTH), NULL, &type, (PBYTE)Value, &(cValue = MAX_KEY_LENGTH));
				if (type == REG_SZ)
				{
					if (wcsstr(Name, str) != NULL)
					{
						*tmpPath = searchResults->path;
						changeCounts(searchResults);

						//_CrtMemDumpAllObjectsSince(&_ms);
						//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
						//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
						return SUBSTR_FOUND;
					}
				}
				j++;
			} while (retCode != ERROR_NO_MORE_ITEMS);

			retCode = RegQueryInfoKey(tmpKey, achClass, &cchClassName, NULL, &cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);

			//переходим к следующим веткам, если ничего не найдено
			if (cSubKeys != 0)
			{
				std::stack <TCHAR*> searchStackTmp;
				searchResults->count.push(cSubKeys);
				for (int i = 0; i < cSubKeys; i++)
				{
					//заполняем стэк подветками текущей ветки
					cbName = MAX_KEY_LENGTH;
					retCode = RegEnumKeyEx(tmpKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
					TCHAR *p = new TCHAR[MAX_KEY_LENGTH];
					wcscpy(p, achKey);
					searchStackTmp.push(p);
					//searchResults->searchStack.push(p);
				}
				
				for (int i = 0; i < cSubKeys; i++)
				{
					searchResults->searchStack.push(searchStackTmp.top());
					searchStackTmp.pop();
				}
			}
			else
			{
				changeCounts(searchResults);
			}
		}
	}
	//_CrtMemDumpAllObjectsSince(&_ms);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	return SUBSTR_NOT_FOUND;
}


void changeCounts(SearchResults *searchResults)
{
	//Удаляем верхний элемент стэка пути из стэка и памяти
	//TCHAR *d = searchResults->path.top();
	searchResults->path.pop();
	//delete d;
	//уменьшаем количество непросмотренных элементов на текущем уровне на 1
	int t = searchResults->count.top();
	t--;
	searchResults->count.pop();
	searchResults->count.push(t);
	//если после уменьшения оказалось, что на текущем уровне не просмотрено 0 
	//элементов (т.е. просмотрены все элементы)
	//то уменьшаем количество непросмотренных элементов на уровень выше
	//если после такого уменьшения непросмотренных элементов уровнем выше не осталось
	//то повторяем операцию заново с более верхним уровнем и т.д.
	while ((searchResults->count.top()) == 0)
	{
		searchResults->count.pop();
		t = searchResults->count.top();
		t--;
		searchResults->count.pop();
		searchResults->count.push(t);
		//TCHAR *d = searchResults->path.top();
		searchResults->path.pop();
		//delete d;
	}
}
//конструируем полный путь до текущей ветки по данным стэка
void reconstructFullPath(SearchResults searchResults, TCHAR fullPath[MAX_KEY_LENGTH])
{
	//Делаем инверсию стэка чтобы был правильный путь
	std::stack <TCHAR*> s;

	while (!searchResults.path.empty())
	{
		TCHAR *str = searchResults.path.top();
		s.push(str);
		searchResults.path.pop();
	}
	//получаем путь до текущей ветки
	TCHAR tmp[MAX_KEY_LENGTH];
	while (!s.empty())
	{
		wcscpy(tmp, s.top());
		wcscat(fullPath, tmp);
		wcscat(fullPath, L"\\");
		s.pop();
	}
}