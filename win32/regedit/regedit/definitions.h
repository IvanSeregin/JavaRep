#pragma once
#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <windows.h> 
#include <stdlib.h> 
#include <string> 
#include <tchar.h>  
#include <CommCtrl.h>
#include <Shlobj.h>
//константы
#define MAX_KEY_LENGTH 4096
#define MAX_VALUE_NAME 16383

//коды контролов
#define REG_TREE_VIEW	100
#define REG_LIST_VIEW	101
#define FILE_DUMP		102
#define FILE_SAVE_CURR	103
#define FILE_EXIT		104
#define FIND_FIND		105
#define DEL_KEY			106 //Удалить ключ
#define ADD_BRANCH		107 //Добавить ветку
#define DEL_BRANCH		108 //Удалить ветку
#define EDIT_BRANCH		109 //Редактировать ветку
#define EDIT_KEY		110 //Редактировать ключ
#define DST_TREEVIEW	111
#define DST_FILE		112
#define ADD_KEY			113 //Добавить ключ

//коды веток реестра
#define HKCR HKEY_CLASSES_ROOT
#define HKCU HKEY_CURRENT_USER
#define HKLM HKEY_LOCAL_MACHINE
#define HKU	 HKEY_USERS
#define HKCC HKEY_CURRENT_CONFIG

//интерфейс
#define ROOT_ITEM_NAME L"Мой компьютер"
#define COL_NAME L"Ключ"
#define COL_TYPE L"Тип"
#define COL_VALUE L"Значение"

struct СurrentItem
{
	HTREEITEM currTreeNode; //указатель на текущий элемент дерева
	//LPNMITEMACTIVATE *currListItem = new LPNMITEMACTIVATE; //указатель на текущий элемент списка
	int currListItem = -1;
};

HWND createRegeditTreeView(HWND hWnd, HINSTANCE hInstance);
HWND createRegeditListView(HWND hWnd);
HMENU createMenu(HWND hWnd);

bool loadRegistry(HWND hWnd, HWND regeditTreeView, HTREEITEM *root); //отображает корневые элементы реестра
bool dumpRegistry(); //создает дамп реестра в файл
void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf); //получае полный путь до выбранного каталога
void updateSubCatalogs(HWND hTreeView, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]); //сканирует и добавляет каталоги к выбранному каталогу
void removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH]);//функция удаляет из пути название корневой ветки реестра
void clearBranch(HWND hTreeView, HTREEITEM hItem);
void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]);//выводит список ключей и их параметров
int deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH]);
int deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH]);
#endif //_LAYOUT_H