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
//константы
#define MAX_KEY_LENGTH 4096
#define MAX_VALUE_NAME 16383
#define TYPE_LENGTH 20

//коды контролов
#define REG_TREE_VIEW	100
#define REG_LIST_VIEW	101
#define FILE_DUMP		102 //Создание дампа +
#define FILE_SAVE_CURR	103 //Сохранение ветки +
#define FILE_EXIT		104 //Выход из приложения +
#define FIND_FIND		105 //Поиск в реестре --
#define DEL_PARAM		106 //Удалить параметр +
#define ADD_BRANCH		107 //Добавить ветку +
#define DEL_BRANCH		108 //Удалить ветку +
#define EDIT_BRANCH		109 //Редактировать ветку +
#define EDIT_PARAM		110 //Редактировать параметр +
#define DST_TREEVIEW	111
#define DST_FILE		112
#define ADD_PARAM		113 //Добавить параметр +
#define EDIT_PARAM_VALUE 114 //Редактировать значение параметра +
#define LV_EDIT			115 //едит для измеения значения параметра
#define ADD_REG_SZ		116 //Добавить текстовый параметр+
#define ADD_REG_DWORD	117 //Добавить dwrod параметр+

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
bool dumBranch(TCHAR path[MAX_KEY_LENGTH]); //создает дамп ветки реестра в файл
void GetFullPath(HTREEITEM hItem, HTREEITEM *root, HWND hTreeView, LPWSTR buf); //получае полный путь до выбранного каталога
void updateSubCatalogs(HWND hTreeView, TV_ITEMW Parent, TCHAR fullPath[MAX_KEY_LENGTH]); //сканирует и добавляет каталоги к выбранному каталогу
TCHAR* removeHKRoot(TCHAR fullPath[MAX_KEY_LENGTH]);//функция удаляет из пути название корневой ветки реестра
void clearBranch(HWND hTreeView, HTREEITEM hItem);//удаляет все подэлементы текущего узла, используется при сворачивании ветки
void enumKeys(HWND hListView, TCHAR fullPath[MAX_KEY_LENGTH]);//выводит список ключей и их параметров
LRESULT deleteParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR keyName[MAX_KEY_LENGTH]);//удаляет параметр реестра
LRESULT deleteBranch(TCHAR fullPath[MAX_KEY_LENGTH]);//удаляет ветку реестра
LRESULT renameBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH]);
LRESULT renameParam(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR oldName[MAX_KEY_LENGTH], TCHAR newName[MAX_KEY_LENGTH], TCHAR Value[MAX_KEY_LENGTH], TCHAR type[TYPE_LENGTH]);//Переименование параметра в реестре
LRESULT addBranch(TCHAR fullPath[MAX_KEY_LENGTH], TCHAR newBranchName[MAX_KEY_LENGTH]);
TV_ITEM getCurrentItem(HWND hTreeView, HTREEITEM hItem); //получает текущий узел дерева
HKEY dumpCatalog(HKEY hRootKey, HKEY hKey, TCHAR path[], HANDLE hFile); //сохранение указанного каталога в файл
HKEY determineHKEY(TCHAR path[MAX_KEY_LENGTH]); // определяет ветку реестра по полному пути до каталога
HTREEITEM insertInTreeView(HWND hWnd, HTREEITEM parent, TCHAR achKey[MAX_KEY_LENGTH]); //вставляет новый подкаталог в текущий каталог
void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH]); //вставляет новую строку в ListView
HKEY openKey(TCHAR fullPath[MAX_KEY_LENGTH]);//открыть ветку
int regValueType(TCHAR type[TYPE_LENGTH]);//определить тип параметра
unsigned int parseTcharToInt(TCHAR value[MAX_KEY_LENGTH]);//перевод текста в целое число (для DWORD параметра типа 0x00000001 (1))
void insertRow(HWND hlistView, TCHAR name[MAX_KEY_LENGTH], TCHAR type[MAX_KEY_LENGTH], TCHAR value[MAX_KEY_LENGTH]);// вставка новой записи в список
#endif //_LAYOUT_H