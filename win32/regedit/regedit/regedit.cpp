#include "definitions.h" 
#include <stdio.h>
#include <string>

//������� ��������� ���� ������ � ����
bool dumpRegistry()
{
	HKEY key;
	TCHAR fileName[MAX_KEY_LENGTH] = _T("");
	PWSTR fName;
	//�������� ���� �� �������� ����� �������� ������������
	//� ���������� ��� ����� ��� ���������� �������
	if (SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &fName) == S_OK)
	{
		wcscpy(fileName, fName);
		wcscat_s(fileName, _T("\\regedit.txt"));
	}
	else
		return false;
	
	//������� ������� ���������� ����
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//wfstream out("events.txt", wios::out);
	
	if (!hFile) return false;
	
	//���� ���� ��� ������ ������, ��������� �� ������ ������ ����� ������� 
	//� �������� ������� ���������� ������� dumpKey(). 
	//������ ������� �������� �����������!!!
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
	//�������� ���� �� �������� ����� �������� ������������
	//� ���������� ��� ����� ��� ���������� �������
	if (SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &fName) == S_OK)
	{
		wcscpy(fileName, fName);
		wcscat_s(fileName, _T("\\regedit.txt"));
	}
	else
		return false;

	//������� ������� ���������� ����
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile) return false;
	//�������� ��������� �� �������� �������
	HKEY hRootKey = determineHKEY(path);
	//�������� ��� ��������� �������� � ��������� ����
	hkeyStr = removeHKRoot(path);
	//���������� ��� ��������� �������� � ����
	wcstombs(nm, hkeyStr, MAX_KEY_LENGTH);
	DWORD written = 0;
	WriteFile(hFile, nm, strlen(nm), &written, NULL);
	WriteFile(hFile, "\n", strlen("\n"), &written, NULL);
	delete[] hkeyStr;
	//��������� ����� ������� � ����
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
	TCHAR    achKey[MAX_KEY_LENGTH];   // ����� ��� ������������
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

	// �������� ���������� � ������� �����
	retCode = RegQueryInfoKey(hKey, achClass, &cchClassName, NULL,&cSubKeys, &cbMaxSubKey, &cchMaxClass, &cValues, &cchMaxValue, &cbMaxValueData, &cbSecurityDescriptor, &ftLastWriteTime);       

	 // cSubKeys - ���������� �������� ������� �����
	if (cSubKeys)
	{
		//---------------------------------------------��������� �������� ����� ������� ------------------
		//���������� ��� ��������
 		for (i = 0; i<cSubKeys; i++)
		{
			TCHAR tmpPath[MAX_KEY_LENGTH] = _T(""); //��� ���������� ���������� ���� �� ��������
			wcscat_s(tmpPath, path); //���������� ������� ��������
			cbName = MAX_KEY_LENGTH;
			//�������� ������ �������� ������� �����, ������� ������� � hKey
			retCode = RegEnumKeyEx(hKey, i,	achKey,	&cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//���� ���������� ��������, �� ���������� ������� ��� ��������
			if (retCode == ERROR_SUCCESS)
			{

				if (wcslen(path)) wcscat_s(tmpPath, _T("\\"));
				wcscat_s(tmpPath, achKey);

				HKEY tmpKey = hKey;
				//��������� ������� ����������
				if (RegOpenKeyEx(hRootKey, tmpPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS)
				{
					//���� ��� ��, �� ���������� � ��� ���������� � ����.
					char *ch = new char[MAX_KEY_LENGTH];
					wcstombs(ch, tmpPath, MAX_KEY_LENGTH);

					WriteFile(hFile, "[", strlen("["), &written, NULL);
					WriteFile(hFile, ch, strlen(ch), &written, NULL);
					WriteFile(hFile, "]\n", strlen("]\n"), &written, NULL);
					
					//---------------------------------------��������� ������ � ������� �����
					//����� ������ ��� ����� � �� �������� �� ������� ����� ������
					LONG lResult;
					DWORD j = 0;
					do
					{
						TCHAR Name[MAX_KEY_LENGTH]; //��� �����
						TCHAR Value[MAX_KEY_LENGTH]; //�������� �����
						DWORD cName = 4096;
						DWORD cValue = 4096;
						DWORD type =  0;

						lResult = RegEnumValue(tmpKey, j, Name, &(cName = MAX_KEY_LENGTH), NULL, &type, (PBYTE)Value, &(cValue = MAX_KEY_LENGTH));
						if (lResult == ERROR_SUCCESS)
						{
							//���� ���� ���� - ����� ��� � ����
							char *vl = new char[MAX_KEY_LENGTH];
							//memset(vl, 0x0, sizeof(vl));
							if (type == REG_DWORD) // �������� ����� ���� REG_DWORD
							{							
								//_ultoa(*Value, vl, 16);
								DWORD *pdwd = new DWORD;
								DWORD type = REG_DWORD;//��� ��������� - DWORD
								DWORD datalen = sizeof(pdwd);
								RegQueryValueEx(tmpKey, Name, 0, &type, (BYTE*)(pdwd), &datalen);
								wsprintf(Value, L"0x%08x (%d) ", *pdwd, *pdwd);
								wcstombs(vl, Value, MAX_KEY_LENGTH);
								//insertRow(hListView, Name, _T("REG_DWORD"), Value);
								delete pdwd;
							}
							else if (type == REG_BINARY) //�������� ����� ���� REG_BINARY
							{
								ultoa(*Value, vl, 16);
							}
							else //�������� ����� ���������
							{
								wcstombs(vl, Value, MAX_KEY_LENGTH);
							}

							//������������ TCHAR � char ��� ������ � ����
							char *nm = new char[MAX_KEY_LENGTH];
							wcstombs(nm, Name, MAX_KEY_LENGTH);

							//���������� ��� ��������� � ����
							if (strlen(nm) == 0)
								WriteFile(hFile, "@", strlen("@"), &written, NULL);
							else
								WriteFile(hFile, nm, strlen(nm), &written, NULL);

							//���������� �������� ��������� � ����
							WriteFile(hFile, "=", strlen("="), &written, NULL);
							WriteFile(hFile, vl, strlen(vl), &written, NULL);
							WriteFile(hFile, "\n", strlen("\n"), &written, NULL);

							//����������� � ������
							delete[] nm;
							delete[] vl;
						}
						j++; //��������� � ���������� ���������
					} while (lResult != ERROR_NO_MORE_ITEMS); //���� ���������� ������ ��� - �������
					///---------------------------------------��������� ������ � ������� ����� ���������
					//����������� � ������
					delete[] ch;
					//�������� ������� ������� ��������, �� ��� ��� ������ ������������ �������� ��������
					dumpCatalog(hRootKey, tmpKey, tmpPath, hFile);
					RegCloseKey(tmpKey);
					WriteFile(hFile, "\n", strlen("\n"), &written, NULL);
				}
			}
		}
	}
	
	return NULL;
}

void updateSubCatalogs(HWND hWnd, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]) //��������� � ��������� �������� � ���������� ��������
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // ����� ��� ������������
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
			//�������� ������ �������� ������� �����, ������� ������� � hKey
			retCode = RegEnumKeyEx(key, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			//���� ���������� ��������, �� ���������� ������� ��� ��������
			
			HTREEITEM Parent2lvl; //�������� ��� ������� ������ ����� ������
			HKEY key2lvl; //����� ������� ������
			TCHAR fullPath2lvl[MAX_KEY_LENGTH]; //���� �� ��������� ������� ������
			DWORD cSubKey2lvl; //���������� ��������� ������� ������

			if (retCode == ERROR_SUCCESS)
			{
				Parent2lvl = insertInTreeView(hWnd, Parent.hItem, achKey);
			}
			
			//��������� �������� ������� ������ ��� �������� ��������
			//�� ������ � ��� ������ ���� ����� ��� �������� ������ ���� ������
			//��� ����� ����� �������� ��������  + ����� ����� ������������ ��������
			
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
						//�������� ������ �������� ������� �����, ������� ������� � hKey
						retCode = RegEnumKeyEx(key2lvl, j, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
						//������ ���������� ����� � �������� �����������
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

void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]) //������� ������ ���������� � �� ��������
{
	ListView_DeleteAllItems(hListView);
	TCHAR    achKey[MAX_KEY_LENGTH];   // ����� ��� ������������
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
			TCHAR Name[MAX_KEY_LENGTH]; //��� �����
			TCHAR Value[MAX_KEY_LENGTH]; //�������� �����
			DWORD cName = 4096;
			DWORD cValue = 4096;
			DWORD type = 0;

			lResult = RegEnumValue(key, j, Name, &(cName = MAX_KEY_LENGTH), NULL, &type, (PBYTE)Value, &(cValue = MAX_KEY_LENGTH));
			if (lResult == ERROR_SUCCESS)
			{
				if (type == REG_DWORD) // �������� ����� ���� REG_DWORD
				{
					DWORD *pdwd = new DWORD;
					DWORD type = REG_DWORD;//��� ��������� - DWORD
					DWORD datalen = sizeof(pdwd);
					RegQueryValueEx(key, Name, 0, &type, (BYTE*)(pdwd), &datalen);
					wsprintf(Value, L"0x%08x (%d) ", *pdwd, *pdwd);
					//wsprintf(Value, L"0x%08x", *pdwd);
					insertRow(hListView, Name, _T("REG_DWORD"), Value);
					delete pdwd;

				}
				else if (type == REG_BINARY) //�������� ����� ���� REG_BINARY
				{
					insertRow(hListView, Name, _T("REG_BINARY"), Value);
				}
				else //�������� ����� ���������
				{
					insertRow(hListView, Name, _T("REG_SZ"), Value);
				}
			}
			j++; //��������� � ���������� ���������
		} while (lResult != ERROR_NO_MORE_ITEMS); //���� ���������� ������ ��� - �������

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
	//��������� ������ ���������
	HKEY hKey = determineHKEY(fullPath);
	HKEY key;
	HKEY keyToDel = openKey(fullPath);

	//������� ������ ��������
	RegDeleteValue(keyToDel, oldName);

	//������� ����� �������� newValue �� ���������� value
	removeHKRoot(fullPath);
	//��������� ������ ����� � ��������������� �������
	LRESULT result = RegOpenKeyEx(hKey, fullPath, 0, KEY_SET_VALUE, &key);
	//���������� ��� ���������
	int typeInt = regValueType(type);
	if (typeInt == REG_SZ)
	{
		//���������� ��������� ��������
		result = SHSetValue(hKey, fullPath, newName, typeInt, Value, wcslen(Value) * 2);
	}
	else if (typeInt == REG_DWORD)
	{
		//���������� �������� ���� dword
		//������ �����, ����� �� ���� �������� � ������� � ����������� � �����
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
		TCHAR    achKey[MAX_KEY_LENGTH];   // ����� ��� ������������
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

		//�������� ��������� ������� ��� ���������
		catalog = searchResults->searchStack.top();
		searchResults->searchStack.pop();
		//����� ��� � ���� ����
		searchResults->path.push(catalog);
		//�������� ������ ���� �� �������� ��������
		reconstructFullPath(*searchResults, fullPath);
		//�������� ��������� �� �������� ������� �������
		HKEY hKey = determineHKEY(fullPath);
		//������� �� ���� ��� ��������� �������� �������
		TCHAR *tmp = removeHKRoot(fullPath);
		delete[] tmp;
		HKEY tmpKey;
		//��������� ������� �����
		if (RegOpenKeyEx(hKey, fullPath, 0, KEY_READ, &tmpKey) == ERROR_SUCCESS);
		{
			//��������� ������� ����� �� ������� ������� � ��� ������� ��������� 
			if (wcsstr(fullPath, str) != NULL)
			{
				*tmpPath = searchResults->path;
				changeCounts(searchResults);
				return SUBSTR_FOUND;
			}
			//��������� ��������� ������� ����� �� ������� ������� � ��� ������� ���������
			int j = 0;
			do
			{
				TCHAR Name[MAX_KEY_LENGTH]; //��� �����
				TCHAR Value[MAX_KEY_LENGTH]; //�������� �����
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

			//��������� � ��������� ������, ���� ������ �� �������
			if (cSubKeys != 0)
			{
				std::stack <TCHAR*> searchStackTmp;
				searchResults->count.push(cSubKeys);
				for (int i = 0; i < cSubKeys; i++)
				{
					//��������� ���� ���������� ������� �����
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
	//������� ������� ������� ����� ���� �� ����� � ������
	//TCHAR *d = searchResults->path.top();
	searchResults->path.pop();
	//delete d;
	//��������� ���������� ��������������� ��������� �� ������� ������ �� 1
	int t = searchResults->count.top();
	t--;
	searchResults->count.pop();
	searchResults->count.push(t);
	//���� ����� ���������� ���������, ��� �� ������� ������ �� ����������� 0 
	//��������� (�.�. ����������� ��� ��������)
	//�� ��������� ���������� ��������������� ��������� �� ������� ����
	//���� ����� ������ ���������� ��������������� ��������� ������� ���� �� ��������
	//�� ��������� �������� ������ � ����� ������� ������� � �.�.
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
//������������ ������ ���� �� ������� ����� �� ������ �����
void reconstructFullPath(SearchResults searchResults, TCHAR fullPath[MAX_KEY_LENGTH])
{
	//������ �������� ����� ����� ��� ���������� ����
	std::stack <TCHAR*> s;

	while (!searchResults.path.empty())
	{
		TCHAR *str = searchResults.path.top();
		s.push(str);
		searchResults.path.pop();
	}
	//�������� ���� �� ������� �����
	TCHAR tmp[MAX_KEY_LENGTH];
	while (!s.empty())
	{
		wcscpy(tmp, s.top());
		wcscat(fullPath, tmp);
		wcscat(fullPath, L"\\");
		s.pop();
	}
}