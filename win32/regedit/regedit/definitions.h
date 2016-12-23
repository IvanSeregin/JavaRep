#pragma once
#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <windows.h> 
#include <stdlib.h> 
#include <string> 
#include <tchar.h>  
#include <CommCtrl.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include "SearchDialog.h"
#include "resource.h"
//���������
#define MAX_KEY_LENGTH 4096
#define MAX_VALUE_NAME 16383
#define TYPE_LENGTH 20

//���� ���������
#define REG_TREE_VIEW	100
#define REG_LIST_VIEW	101
#define FILE_DUMP		102 //�������� ����� +
#define FILE_SAVE_CURR	103 //���������� ����� +
#define FILE_EXIT		104 //����� �� ���������� +
#define FIND_FIND		105 //����� � ������� --
#define DEL_PARAM		106 //������� �������� +
#define ADD_BRANCH		107 //�������� ����� +
#define DEL_BRANCH		108 //������� ����� +
#define EDIT_BRANCH		109 //������������� ����� +
#define EDIT_PARAM		110 //������������� �������� +
#define DST_TREEVIEW	111
#define DST_FILE		112
#define ADD_PARAM		113 //�������� �������� +
#define EDIT_PARAM_VALUE 114 //������������� �������� ��������� +
#define LV_EDIT			115 //���� ��� �������� �������� ���������
#define ADD_REG_SZ		116 //�������� ��������� ��������+
#define ADD_REG_DWORD	117 //�������� dwrod ��������+

//���� ����� �������
#define HKCR HKEY_CLASSES_ROOT
#define HKCU HKEY_CURRENT_USER
#define HKLM HKEY_LOCAL_MACHINE
#define HKU	 HKEY_USERS
#define HKCC HKEY_CURRENT_CONFIG

//���������
#define ROOT_ITEM_NAME L"��� ���������"
#define COL_NAME L"����"
#define COL_TYPE L"���"
#define COL_VALUE L"��������"

struct �urrentItem
{
	HTREEITEM currTreeNode; //��������� �� ������� ������� ������
	//LPNMITEMACTIVATE *currListItem = new LPNMITEMACTIVATE; //��������� �� ������� ������� ������
	int currListItem = -1;
};

HWND createRegeditTreeView(HWND hWnd, HINSTANCE hInstance);
HWND createRegeditListView(HWND hWnd);
HMENU createMenu(HWND hWnd);

bool loadRegistry(HWND hWnd, HWND regeditTreeView, HTREEITEM *root); //���������� �������� �������� �������
bool dumpRegistry(); //������� ���� ������� � ����
bool dumBranch(TCHAR path[MAX_KEY_LENGTH]); //������� ���� ����� ������� � ����
void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf); //������� ������ ���� �� ���������� ��������
void updateSubCatalogs(HWND hTreeView, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]); //��������� � ��������� �������� � ���������� ��������
TCHAR* removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH]);//������� ������� �� ���� �������� �������� ����� �������
void clearBranch(HWND hTreeView, HTREEITEM hItem);//������� ��� ����������� �������� ����, ������������ ��� ������������ �����
void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]);//������� ������ ������ � �� ����������
LRESULT deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH]);//������� �������� �������
LRESULT deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH]);//������� ����� �������
LRESULT renameBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH]);
LRESULT renameParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR oldName[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH], TCHAR Value[MAX_KEY_LENGTH], TCHAR type[TYPE_LENGTH]);//�������������� ��������� � �������
LRESULT addBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newBranchName[MAX_KEY_LENGTH]);
TV_ITEM getCurrentItem(HWND hTreeView, HTREEITEM hItem); //�������� ������� ���� ������
HKEY dumpCatalog(HKEY hRootKey, HKEY hKey, TCHAR path[], HANDLE hFile); //���������� ���������� �������� � ����
HKEY determineHKEY(TCHAR path[MAX_KEY_LENGTH]); // ���������� ����� ������� �� ������� ���� �� ��������
HTREEITEM insertInTreeView(HWND hWnd, HTREEITEM parent, TCHAR achKey[MAX_KEY_LENGTH]); //��������� ����� ���������� � ������� �������
void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH]); //��������� ����� ������ � ListView
HKEY openKey(TCHAR fullPath[MAX_KEY_LENGTH]);//������� �����
int regValueType(TCHAR type[TYPE_LENGTH]);//���������� ��� ���������
unsigned int parseTcharToInt(TCHAR value[MAX_KEY_LENGTH]);//������� ������ � ����� ����� (��� DWORD ��������� ���� 0x00000001 (1))
void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH]);// ������� ����� ������ � ������
#endif //_LAYOUT_H