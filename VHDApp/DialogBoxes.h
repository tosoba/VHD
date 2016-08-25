#pragma once
#include <Windows.h>
#include "resources.h"
#include "StringOperations.h"

using namespace std;

bool SetupDiskCreation(HWND hwndDlg, wstring& wstrFullDiskPath)
{
	HWND hwndEditDiskName = GetDlgItem(hwndDlg, ID_EDIT_DISKNAME);
	string strDiskName = WindowTextToString(hwndEditDiskName);
	if (!CheckIfStringEmpty(strDiskName, L"Enter disk name.", hwndDlg, hwndEditDiskName))
		return false;
	if (strDiskName.find(".vhd") != strDiskName.size() - 4)
		strDiskName.append(".vhd");

	HWND hwndEditDiskSize = GetDlgItem(hwndDlg, ID_EDIT_DISKSIZE);
	string strDiskSize = WindowTextToString(hwndEditDiskSize);
	if (!CheckIfStringEmpty(strDiskSize, L"Enter disk size (in MB).", hwndDlg, hwndEditDiskSize))
		return false;
	if (!CheckIfStringContainsNumbersOnly(strDiskSize, L"Enter a valid disk size - numbers only.", hwndDlg, hwndEditDiskSize))
		return false;
	int size = stoi(strDiskSize);

	HWND hwndEditFolderPath = GetDlgItem(hwndDlg, ID_EDIT_DISKFOLDER);

	string strFolderName = WindowTextToString(hwndEditFolderPath);

	string strFullDiskPath = strFolderName + "\\" + strDiskName;
	wstrFullDiskPath = toWString(strFullDiskPath);

	return true;
}

INT_PTR CALLBACK NewDiskDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LPCWSTR path;
	static HWND hwndEditFolderPath;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		path = (LPCWSTR)lParam;
		hwndEditFolderPath = GetDlgItem(hwndDlg, ID_EDIT_DISKFOLDER);
		SetWindowText(hwndEditFolderPath, path);
		break;

	case WM_COMMAND:

		if (LOWORD(wParam == ID_BTN_CREATE_AND_MOUNT))
		{
			wstring wstrFullDiskPath;
			if (!SetupDiskCreation(hwndDlg, wstrFullDiskPath))
				break;
			//commented out for safety :p
			/*CreateVHD_Fixed(&wstrFullDiskPath[0], size);
			OpenAndAttachVHD2(&wstrFullDiskPath[0], CountPhysicalDisks());*/
			EndDialog(hwndDlg, ID_BTN_CREATE_AND_MOUNT);

			break;
		}

		if (LOWORD(wParam == ID_BTN_CREATE))
		{
			wstring wstrFullDiskPath;
			if (!SetupDiskCreation(hwndDlg, wstrFullDiskPath))
				break;
			//commented out for safety :p
			//CreateVHD_Fixed(&wstrFullDiskPath[0], size);
			EndDialog(hwndDlg, ID_BTN_CREATE_AND_MOUNT);
		}

		break;

	case WM_QUIT:
		EndDialog(hwndDlg, 0);
		break;
	case WM_DESTROY:
		EndDialog(hwndDlg, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK ChooseDiskDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndCombo;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hwndCombo = GetDlgItem(hwndDlg, ID_COMBO_DISK);
		AddItemsToCombobox(hwndCombo, GetDriveLetters());
		CheckDlgButton(hwndDlg, ID_CHECKBOX_SORT, BST_UNCHECKED);
		CheckDlgButton(hwndDlg, ID_CHECKBOX_ENCRYPT, BST_UNCHECKED);
		break;

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_BTN_CREATE_FOLDERS)
		{
			bool sortFolder = IsDlgButtonChecked(hwndDlg, ID_CHECKBOX_SORT);
			bool encryptFolder = IsDlgButtonChecked(hwndDlg, ID_CHECKBOX_ENCRYPT);
			hwndCombo = GetDlgItem(hwndDlg, ID_COMBO_DISK);
			string diskPath = TextFromComboboxToString(hwndCombo);
			if (!CheckIfStringEmpty(diskPath, L"Choose a disk.", hwndDlg, NULL))
				break;

			if (sortFolder)
			{
				string strSortFolderPath = diskPath + "Sort";
				wstring wstrSortFolderPath = toWString(strSortFolderPath);
				CreateDirectory(&wstrSortFolderPath[0], NULL);
			}

			if (encryptFolder)
			{
				string strEncryptFolderPath = diskPath + "Encrypt";
				wstring wstrEncryptFolderPath = toWString(strEncryptFolderPath);
				CreateDirectory(&wstrEncryptFolderPath[0], NULL);
			}

			if (!sortFolder && !encryptFolder)
			{
				MessageBox(hwndDlg, L"Choose a folder.", L"Error", MB_OK);
				break;
			}

			//TODO : return diskPath somehow through global var I guess
			g_diskPath = diskPath;

			EndDialog(hwndDlg, ID_BTN_CREATE_FOLDERS);
			break;
		}

		break;

	case WM_QUIT:
		EndDialog(hwndDlg, 0);
		break;
	case WM_DESTROY:
		EndDialog(hwndDlg, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK NoFoldersDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:

		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BTN_CHOOSE_DISK)
		{
			DialogBox(g_hinst, MAKEINTRESOURCE(IDD_CHOOSEDISK), hwndDlg, (DLGPROC)ChooseDiskDialogProc);
			EndDialog(hwndDlg, 0);
			break;
		}

		if (LOWORD(wParam) == ID_BTN_CANCEL)
		{
			EndDialog(hwndDlg, 0);
			break;
		}
		
		break;

	case WM_QUIT:
		EndDialog(hwndDlg, 0);
		break;
	case WM_DESTROY:
		EndDialog(hwndDlg, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}