//https://www.napishem.com/customer-248717.html


#include "definitions.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setupWindow(HWND hWnd, HINSTANCE hInstance);

HWND regeditTreeView; //������� ��� ����������� ������ �������
HWND regeditListView; // ������� ��� ����������� �������� ��������� �����
HMENU mainMenu; //���� ����������
HTREEITEM *root; //�������� ������� ��� TreeView
�urrentItem currItem; //��������� ��� �������� �������� ���� ������ � �������� ������
HWND *htvEdit = new HWND; // ������������� ���� ������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_CTYPE, "Russian");
	WNDCLASSEX wcex;
	static TCHAR szWindowClass[] = _T("regedit");
	static TCHAR szTitle[] = _T("regedit");
	//htvEdit = NULL;
	
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
	
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, hInstance, NULL);
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
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_DESTROY:
			delete htvEdit;
			PostQuitMessage(0);
			break;
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
			//------------------------------------ �������������� �������� -------------------------
			if (LOWORD(wParam) == EDIT_BRANCH) //����� ����� ���� ������������� �������
			{
				*htvEdit = TreeView_EditLabel(regeditTreeView, currItem.currTreeNode);
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