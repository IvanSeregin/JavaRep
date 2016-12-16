#pragma once
#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <windows.h> 
#include <CommCtrl.h>

#define REG_TREE_VIEW 100
#define REG_LIST_VIEW 101
#define FILE_DUMP 102
#define FILE_SAVE_CURR 103
#define FILE_EXIT 104
#define FIND_FIND 105
#define EDIT_EDIT 106
#define EDIT_ADD 107
#define EDIT_DEL 108
#define EDIT_BRANCH 109
#define EDIT_KEY 110

#define BTN_CANCEL 111

HWND createRegeditTreeView(HWND hWnd);
HWND createRegeditListView(HWND hWnd);
HMENU createMenu(HWND hWnd);

HWND createBtn(HWND hWnd);
#endif //_LAYOUT_H