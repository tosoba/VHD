#pragma once
#include <Windows.h>
#include "Visuals.h"
#include "Conversions.h"
#include "Combobox.h"

#define ID_BUTTON_CREATE_AND_MOUNT 4
#define ID_BUTTON_CREATE_FOLDERS 8
#define ID_BUTTON_CREATE 14
#define ID_COMBOBOX 16
#define ID_CHECKBOX_SORT_FOLDER 30
#define ID_CHECKBOX_ENCRYPT_FOLDER 31

HWND CreateDialogBox(HWND hwndParent, HINSTANCE hInstance, LPCWSTR param, LPCWSTR lpClassName, LPCWSTR title, int x = 100, int y = 100, int width = 300, int height = 200);
void RegisterDialogClass(HWND hwnd, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc);

LRESULT CALLBACK DiskDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateFoldersDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK CreateFoldersDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndCombo, hwndButtonChooseDisk;

	switch (msg) {

	case WM_CREATE:

		CreateWindowW(L"static", L"Disk:",
			WS_CHILD | WS_VISIBLE, 10, 14, 35, 25, hwnd,
			(HMENU)6, NULL, NULL);

		hwndCombo = CreateWindowW(L"Combobox", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			55, 12, 50, 65, hwnd, (HMENU)ID_COMBOBOX, NULL, NULL);

		AddItemsToCombobox(hwndCombo, GetDriveLetters());

		hwndButtonChooseDisk = CreateWindowW(L"button", L"Create folders",
			WS_VISIBLE | WS_CHILD, 115, 10, 90, 25,
			hwnd, (HMENU)ID_BUTTON_CREATE_FOLDERS, NULL, NULL);

		CreateWindowW(L"button", L"Sort folder",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 40, 100, 20, hwnd, (HMENU)ID_CHECKBOX_SORT_FOLDER,
			NULL, NULL);

		CreateWindowW(L"button", L"Encrypt folder",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 60, 100, 20, hwnd, (HMENU)ID_CHECKBOX_ENCRYPT_FOLDER,
			NULL, NULL);

		CheckDlgButton(hwnd, ID_CHECKBOX_SORT_FOLDER, BST_UNCHECKED);
		CheckDlgButton(hwnd, ID_CHECKBOX_ENCRYPT_FOLDER, BST_UNCHECKED);

		EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

		break;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_BUTTON_CREATE_FOLDERS)
		{
			if (IsDlgButtonChecked(hwnd, ID_CHECKBOX_SORT_FOLDER))
			{
				string diskPath = TextFromComboboxToString(hwndCombo);
				if (diskPath.size() == 0)
				{
					MessageBox(hwnd, L"Choose a disk.", L"Error", MB_OK);
					break;
				}
				string strSortFolderPath = diskPath + "Sort";
				wstring wstrSortFolderPath = toWString(strSortFolderPath);
				CreateDirectory(&wstrSortFolderPath[0], NULL);
			}

			if (IsDlgButtonChecked(hwnd, ID_CHECKBOX_ENCRYPT_FOLDER))
			{
				string diskPath = TextFromComboboxToString(hwndCombo);
				if (diskPath.size() == 0)
				{
					MessageBox(hwnd, L"Choose a disk.", L"Error", MB_OK);
					break;
				}
				string strEncryptFolderPath = diskPath + "Encrypt";
				wstring wstrEncryptFolderPath = toWString(strEncryptFolderPath);
				CreateDirectory(&wstrEncryptFolderPath[0], NULL);
			}
		}

		break;
	}
	case WM_CLOSE:

		break;

	case WM_DESTROY:
		break;
	}

	return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

LRESULT CALLBACK DiskDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndButtonCreateAndMount, hwndButtonCreate;
	static LPCWSTR userdata;
	static HWND hwndEditDiskSize, hwndEditDiskName, hwndEditFolderPath;

	switch (msg) {

	case WM_CREATE:

		hwndEditDiskName = CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			45, 30, 220, 20, hwnd, (HMENU)1,
			NULL, NULL);

		CreateWindowW(L"static", L"Name:",
			WS_CHILD | WS_VISIBLE, 10, 32, 35, 25, hwnd,
			(HMENU)6, NULL, NULL);

		hwndEditDiskSize = CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			45, 55, 220, 20, hwnd, (HMENU)1,
			NULL, NULL);

		CreateWindowW(L"static", L"Size:",
			WS_CHILD | WS_VISIBLE, 10, 57, 35, 25, hwnd,
			(HMENU)6, NULL, NULL);

		hwndEditFolderPath = CreateWindowW(L"Edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
			45, 80, 220, 20, hwnd, (HMENU)1,
			NULL, NULL);

		CreateWindowW(L"static", L"Folder:",
			WS_CHILD | WS_VISIBLE, 10, 82, 35, 25, hwnd,
			(HMENU)6, NULL, NULL);

		hwndButtonCreate = CreateWindowW(L"button", L"Create",
			WS_VISIBLE | WS_CHILD, 10, 115, 130, 25,
			hwnd, (HMENU)ID_BUTTON_CREATE, NULL, NULL);

		hwndButtonCreateAndMount = CreateWindowW(L"button", L"Create and mount",
			WS_VISIBLE | WS_CHILD, 145, 115, 130, 25,
			hwnd, (HMENU)ID_BUTTON_CREATE_AND_MOUNT, NULL, NULL);

		static CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		userdata = reinterpret_cast<LPCWSTR>(pCreate->lpCreateParams);

		SetWindowText(hwndEditFolderPath, userdata);

		EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

		break;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_BUTTON_CREATE_AND_MOUNT) 
		{
			string strDiskName = WindowTextToString(hwndEditDiskName);

			string strFolderName = WindowTextToString(hwndEditFolderPath);

			string strDiskSize = WindowTextToString(hwndEditDiskSize);
			int size = stoi(strDiskSize);

			string strFullDiskPath = strFolderName + "\\" + strDiskSize;
			wstring wstrFullDiskPath = toWString(strFullDiskPath);

			//TODO : error checking
			//TODO : refresh treeview after mount

			//commented out for safety :p
			/*CreateVHD_Fixed(&wstrFullDiskPath[0], size);
			OpenAndAttachVHD2(&wstrFullDiskPath[0], CountPhysicalDisks());*/
		}

		if (LOWORD(wParam) == ID_BUTTON_CREATE)
		{
			string strDiskName = WindowTextToString(hwndEditDiskName);

			string strFolderName = WindowTextToString(hwndEditFolderPath);

			string strDiskSize = WindowTextToString(hwndEditDiskSize);
			int size = stoi(strDiskSize);

			string strFullDiskPath = strFolderName + "\\" + strDiskSize;
			wstring wstrFullDiskPath = toWString(strFullDiskPath);

			//TODO : error checking

			//commented out for safety :p
			//CreateVHD_Fixed(&wstrFullDiskPath[0], size);
		}

		break;
	}
	case WM_CLOSE:
		break;

	}

	return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

void RegisterDialogClass(HWND hwnd, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc) {

	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = lpfnWndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = lpszClassName;
	RegisterClassExW(&wc);

}

HWND CreateDialogBox(HWND hwndParent, HINSTANCE hInstance, LPCWSTR param, LPCWSTR lpClassName, LPCWSTR title, int x, int y, int width, int height) {

	//EnableWindow(hwndParent, FALSE); 
	return CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, lpClassName, title,
		WS_VISIBLE | WS_SYSMENU | WS_CAPTION, x, y, width, height,
		NULL, NULL, hInstance, (LPVOID)param);
}