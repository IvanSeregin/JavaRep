#pragma once
#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <windows.h> 
#include <stdlib.h> 
#include <string> 
#include <tchar.h>  
#include <CommCtrl.h>
#include <Shlobj.h>
//���������
#define MAX_KEY_LENGTH 4096
#define MAX_VALUE_NAME 16383

//���� ���������
#define REG_TREE_VIEW	100
#define REG_LIST_VIEW	101
#define FILE_DUMP		102
#define FILE_SAVE_CURR	103
#define FILE_EXIT		104
#define FIND_FIND		105
#define DEL_KEY			106 //������� ����
#define ADD_BRANCH		107 //�������� �����
#define DEL_BRANCH		108 //������� �����
#define EDIT_BRANCH		109 //������������� �����
#define EDIT_KEY		110 //������������� ����
#define DST_TREEVIEW	111
#define DST_FILE		112
#define ADD_KEY			113 //�������� ����

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
void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf); //������� ������ ���� �� ���������� ��������
void updateSubCatalogs(HWND hTreeView, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]); //��������� � ��������� �������� � ���������� ��������
void removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH]);//������� ������� �� ���� �������� �������� ����� �������
void clearBranch(HWND hTreeView, HTREEITEM hItem);
void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]);//������� ������ ������ � �� ����������
int deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH]);
int deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH]);
#endif //_LAYOUT_H