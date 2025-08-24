#include "MenuBar.h"
#include <Windows.h>


void MenuBar::CreateMenuBar(GLFWwindow* window, HWND hwnd)
{

	HMENU hMenu = CreateMenu();

	//Creates the first dropdown menu, "File"
	HMENU hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu, MF_STRING, 1, L"Open");
	AppendMenu(hFileMenu, MF_STRING, 2, L"Save");
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hFileMenu, MF_STRING, 3, L"Exit");

	//Creates the second dropdown menu, "Edit"
	HMENU hEditMenu = CreatePopupMenu();
	AppendMenu(hEditMenu, MF_STRING, 1, L"Project Settings");
	AppendMenu(hEditMenu, MF_STRING, 2, L"Editor Settings");

	//Creates the third dropdown menu, "View"
	HMENU hViewMenu = CreatePopupMenu();
	AppendMenu(hViewMenu, MF_STRING, 1, L"Editor Viewmode");

	//Seperates the dropdown, kinda like a horizontal rule (<hr>)
	AppendMenu(hViewMenu, MF_SEPARATOR, 0, NULL);

	AppendMenu(hViewMenu, MF_STRING, 2, L"Editor Camera FOV");
	AppendMenu(hViewMenu, MF_STRING, 3, L"Viewport Resolution");

	AppendMenu(hViewMenu, MF_SEPARATOR, 0, NULL);

	AppendMenu(hViewMenu, MF_STRING, 4, L"Viewport Window");
	AppendMenu(hViewMenu, MF_STRING, 5, L"File Explorer Windows");

	//Creates the fourth dropdown menu, "Documentation"
	HMENU hDoc = CreatePopupMenu();
	AppendMenu(hDoc, MF_STRING, 1, L"Voxl Documentation");
	AppendMenu(hDoc, MF_SEPARATOR, 0, NULL);
	AppendMenu(hDoc, MF_STRING, 2, L"Created by TheMrSnoop");


	//Creates the fourth dropdown menu, "Add"
	HMENU hAdd = CreatePopupMenu();
	AppendMenu(hAdd, MF_STRING, 1, L"Game Object");
	AppendMenu(hAdd, MF_STRING, 2, L"Player Object");
	AppendMenu(hAdd, MF_STRING, 3, L"Character Object");


	//Adds both of the dropdown menus to the window
	//! Note. Instead of just copying and pasting all the previous code I should figure out how to pull the dropdown text from a .json file.
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"View");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDoc, L"Documentation");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hAdd, L"Add");


	SetMenu(hwnd, hMenu);
}