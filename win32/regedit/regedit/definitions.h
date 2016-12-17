#pragma once
#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h>  
#include <CommCtrl.h>
//коды контролов
#define REG_TREE_VIEW	100
#define REG_LIST_VIEW	101
#define FILE_DUMP		102
#define FILE_SAVE_CURR	103
#define FILE_EXIT		104
#define FIND_FIND		105
#define EDIT_EDIT		106
#define EDIT_ADD		107
#define EDIT_DEL		108
#define EDIT_BRANCH		109
#define EDIT_KEY		110

//коды веток реестра
#define HKCR HKEY_CLASSES_ROOT
#define HKCU HKEY_CURRENT_USER
#define HKLM HKEY_LOCAL_MACHINE
#define HKU	 HKEY_USERS
#define HKCC HKEY_CURRENT_CONFIG

HWND createRegeditTreeView(HWND hWnd, HINSTANCE hInstance);
HWND createRegeditListView(HWND hWnd);
HMENU createMenu(HWND hWnd);

bool loadRegistry(HWND hWnd, HWND regeditTreeView);
bool dumpRegistry();

#endif //_LAYOUT_H