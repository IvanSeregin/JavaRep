#include "definitions.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void setupWindow(HWND hWnd, HINSTANCE hInstance);

HWND hWnd; //������� ����
HWND regeditTreeView; //������� ��� ����������� ������ �������
HWND regeditListView; // ������� ��� ����������� �������� ��������� �����
HMENU mainMenu; //���� ����������
HTREEITEM *root; //�������� ������� ��� TreeView
�urrentItem currItem; //��������� ��� �������� �������� ���� ������ � �������� ������
SearchResults *searchResults = new SearchResults;
HWND *htvEdit = new HWND; // ������������� ���� ������
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

	setupWindow(hWnd, hInstance); //������� �������� � ����
	root = new HTREEITEM;
	loadRegistry(hWnd, regeditTreeView, root); //��������� ������ 

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
			//---------------------------------------------------- ����� ---------------------------
			if (LOWORD(wParam) == FILE_EXIT) //����� ����� ���� �����
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			} else
			//---------------------------------------------------- �������� ����� ------------------
			if (LOWORD(wParam) == FILE_DUMP) //����� ����� ���� ������� ����
			{
				dumpRegistry(); //����� ������� �������� �����
			} else
			//---------------------------------------------------- �������� ����� �����------------------
			if (LOWORD(wParam) == FILE_SAVE_CURR) //����� ����� ���� ������� ����
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				dumBranch(fullPath); //����� ������� �������� �����
			}
			else
			//---------------------------------------------------- �������� ��������� --------------
			if (LOWORD(wParam) == DEL_PARAM) //����� ����� ������� ��������
			{
				if (currItem.currListItem == -1)
				{
					MessageBox(hWnd, L"�� ������ �� ���� ��������", L"OK", MB_OK);
					break;
				}
				//�������� ���� �� �������� ��������� �����
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				//�������� ��� �������� ��������� �����
				TCHAR pszText[MAX_KEY_LENGTH];
				ListView_GetItemText(regeditListView, currItem.currListItem, 0, pszText, MAX_KEY_LENGTH);
				if (deleteParam(fullPath, pszText) != ERROR_SUCCESS)
				{
					MessageBox(hWnd, L"������ ��� �������� ���������", L"OK", MB_OK);
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
					MessageBox(hWnd, L"�������� ������ �������", L"OK", MB_OK);
				}
			} else
			//---------------------------------------------------- �������� ����� ������� --------------
			if (LOWORD(wParam) == DEL_BRANCH) //����� ����� ������� �����
			{
				//�������� ���� �� �������� ��������� �����
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				if (deleteBranch(fullPath) != ERROR_SUCCESS)
				{
					MessageBox(hWnd, L"������ ��� �������� �����", L"OK", MB_OK);
				}
				else
				{
					//��������� �����- ��������
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
					MessageBox(hWnd, L"����� ������� �������", L"OK", MB_OK);

				}				
			}
			//--------------------------------------���������� �������� -----------------------
			if (LOWORD(wParam) == ADD_BRANCH)
			{
				//�������� ���� �� �������� ��������
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				//������� �������� ����� �������, ���� ��� ��, �� ��������� ����� ���� � ������
				if (addBranch(fullPath, L"����� ������") == ERROR_SUCCESS)
				{
					insertInTreeView(hWnd, currItem.currTreeNode, L"����� ������");
				}
				else
					MessageBox(hWnd, L"���������� ������� ������", L"OK", MB_OK);
			}
			//------------------------------------ �������������� �������� -------------------------
			if (LOWORD(wParam) == EDIT_BRANCH) //����� ����� ���� ������������� �������
			{
				*htvEdit = TreeView_EditLabel(regeditTreeView, currItem.currTreeNode);
			}
			else
			//------------------------------- �������������� ����� ��������� --------------
			if (LOWORD(wParam) == EDIT_PARAM)
			{
				*htvEdit = ListView_EditLabel(regeditListView, currItem.currListItem); //������ ����������� LVN_ENDLABELEDIT
			}
			//------------------------------- �������������� �������� ��������� --------------
			if (LOWORD(wParam) == EDIT_PARAM_VALUE)
			{
				/*
				�.�. ���������� ������� ��������������� subItem � itema ���, �� ��� subItem-�� �������
				������� edit, �������� � ���� ����� � subItema � ������������� ������� ��������� �� EditProc
				����� ��� ��������� ���������� ���������� ��������� ��� edit�
				*/
				//���������� ��� edit�
				RECT *r1 = new RECT;
				//��������� �������������
				ListView_GetSubItemRect(regeditListView, currItem.currListItem, 2, LVIR_LABEL, r1);
				//����� �������������� �������
				int iCol = 2;
				//��� �������� ������
				TCHAR pszText[MAX_KEY_LENGTH] = _T("");
				//�������� ����� � �������� subItema
				ListView_GetItemText(regeditListView, currItem.currListItem, 2, pszText, MAX_KEY_LENGTH);
				//������� edit
				lvEdit = CreateWindowEx(0,
					WC_EDIT,
					pszText,
					WS_VISIBLE | WS_CHILD | WS_BORDER,
					r1->left + 416, r1->top + 5, r1->right - r1->left-1, r1->bottom - r1->top,
					hWnd,
					(HMENU)LV_EDIT,
					(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
					NULL);
				//����� ������� ���������
				wpRecordProc = (WNDPROC)SetWindowLong(lvEdit, GWL_WNDPROC, (LONG)EditProc);
				//������������� �����
				SetFocus(lvEdit);
				delete r1;
			}
			else
			//------------------------------- �������� ��������� REG_SZ --------------
			if (LOWORD(wParam) == ADD_REG_SZ)
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
				if (renameParam(fullPath, L"����� ��������", L"����� ��������", L"����� ��������", L"REG_SZ") == ERROR_SUCCESS)
				{
					insertRow(regeditListView, L"����� ��������", L"REG_SZ", L"����� ��������");
				}
				else
					MessageBox(hWnd, L"������ ��� �������� ������ ���������", L"��������!", MB_OK);
			}
			else
			//------------------------------- �������� ��������� DWORD --------------
			if (LOWORD(wParam) == ADD_REG_DWORD)
			{
				TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
				GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
 				if (renameParam(fullPath, L"����� ��������", L"����� ��������", L"0x00000000 (0)", L"REG_DWORD") == ERROR_SUCCESS)
				{
					insertRow(regeditListView, L"����� ��������", L"REG_DWORD", L"0x00000000 (0)");
				}
				else
					MessageBox(hWnd, L"������ ��� �������� ������ ���������", L"��������!", MB_OK);

			}
			else
			//------------------------------------- ����� � �������-----------------------
			if (LOWORD(wParam) == FIND_FIND)
			{
				cretaeSearchDlg(hInst, hWnd);
			}
			else
			//------------------------------------- ����� ����� -----------------------
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
				case REG_TREE_VIEW: // ������� ��������� � ��������� TreeView
				{
					//--------------------- ������� �� ��������� ������ � ������
					if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
					{
						HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem; //�������� ��������� �� ���������� ������
						TCHAR fullPath[MAX_KEY_LENGTH]=_T("");
						/*���� �������� ������ �������� �� �����
						�� ������� ������ ���� � ������� �� ����� ��������
						� ����������� ��� ����� � �� �������� � �������� ListView
						*/
						if (hClicked != *root) 
						{
							GetFullPath(hClicked, root, regeditTreeView, fullPath);
							currItem.currTreeNode = hClicked;
							enumKeys(regeditListView, fullPath);
						}
					}
					//---------------- //������� �� �������������� ������ � ������
					else if (((LPNMHDR)lParam)->code == TVN_ITEMEXPANDING) 
					{
						HTREEITEM hClicked = ((NMTREEVIEW*)lParam)->itemNew.hItem;
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						if (((NMTREEVIEW*)lParam)->action == TVE_EXPAND)
						{
							if (hClicked != *root)
							{
								//�������� ���� �� ��������
								GetFullPath(hClicked, root, regeditTreeView, fullPath);
								//�������������� ������� �������� �������� ���� � ������
								clearBranch(regeditTreeView, ((NMTREEVIEW*)lParam)->itemNew.hItem);
								//��������� ���������� ���� ������
								updateSubCatalogs(hWnd, ((NMTREEVIEW*)lParam)->itemNew, fullPath);
							}
							else 
								break;
						}
					}
					//------------------- ������� ��� ��������� ���� ������----------------------------------
					else if (((LPNMHDR)lParam)->code == TVN_ENDLABELEDIT)
					{
						//�������� ���� �� �������� ��������
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
						//��������������� ���� ������
						TV_ITEM currTVItem = getCurrentItem(regeditTreeView, currItem.currTreeNode);
						TCHAR newLabelText[MAX_KEY_LENGTH] = _T("");
						GetWindowText(*htvEdit, newLabelText, MAX_KEY_LENGTH);
						wcscpy(currTVItem.pszText, newLabelText);
						TreeView_SetItem(regeditTreeView, &currTVItem);
						//�������� � �������
						renameBranch(fullPath, newLabelText);
					}
					break;
				}
				case REG_LIST_VIEW:// ������� ��������� � ��������� ListView
				{
					if (((LPNMHDR)lParam)->code == NM_CLICK)//������� �� ��������� ������ � ������ ListView
					{
						//����� ���������� ������
						LPNMITEMACTIVATE  lpnmitem = (LPNMITEMACTIVATE)lParam; //��������� �� ������� ������� � ������
						currItem.currListItem = lpnmitem->iItem;

						//�������� ����� ������
						//TCHAR pszText[MAX_KEY_LENGTH];
						//ListView_GetItemText(regeditListView, currItem.currListItem, 0, pszText, MAX_KEY_LENGTH);

					}
					if (((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT) //���������� ����������� ��������������
					{
						
						//�������� ���� �� �������� ��������
						TCHAR fullPath[MAX_KEY_LENGTH] = _T("");
						TCHAR oldName[MAX_KEY_LENGTH] = _T("");
						TCHAR newName[MAX_KEY_LENGTH] = _T("");
						TCHAR value[MAX_KEY_LENGTH] = _T("");
						TCHAR type[TYPE_LENGTH] = _T("");

						GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);

						//�������� ����� �������� ���������
						GetWindowText(*htvEdit, newName, MAX_KEY_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 0, oldName, MAX_KEY_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 1, type, TYPE_LENGTH);
						ListView_GetItemText(regeditListView, currItem.currListItem, 2, value, MAX_KEY_LENGTH);
						ListView_SetItemText(regeditListView, currItem.currListItem, 0, newName);

						//��������� ����� �������� � �������
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
			//�������� ���� �� �������� ��������
			GetFullPath(currItem.currTreeNode, root, regeditTreeView, fullPath);
			//��������������� �������� � ������
			ListView_SetItemText(regeditListView, currItem.currListItem, 2, value);
			//������ ���� ����
			DestroyWindow(hwnd);
			//��������� ����� �������� � �������
			renameParam(fullPath, oldName, oldName, value, type);
			break;
		}
		}
	}
	}
	return CallWindowProc(wpRecordProc, hwnd, message, wParam, lParam);
}

//������� �������������� ��������� SearchResults ��� ������
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
		//������������� �������� ������ �� ���������� ����, ������� �������� � SearchResults.path
		//������� ����������� ����� ����� SearchResults.path
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
		MessageBox(hWnd, L"������ �� �������!", L"��������!", 0);
		//�������� ������������� ��������� ���������
		//� ����� ������������ ������ � ���������� �� � ������ ������
		initSearch(searchResults);
	}
}