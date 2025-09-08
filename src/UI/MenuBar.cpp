#include "MenuBar.h"
#include "json.hpp"
#include <fstream>
#include <Windows.h>


using json = nlohmann::json;

void MenuBar::CreateMenuBar(GLFWwindow* window, HWND hwnd)
{
    // load the json file
    std::ifstream file("json/MenuBar.json");
    if (!file.is_open()) {
        MessageBox(hwnd, L"Failed to open MenuBar.json", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    json menuData;
    try {
        file >> menuData;
    }
    catch (const std::exception& e) {
        MessageBoxA(hwnd, (std::string("Failed to parse JSON: ") + e.what()).c_str(), "Error", MB_OK | MB_ICONERROR);
        return;
    }


    HMENU hMenu = CreateMenu();
    UINT commandId = 1; // assign unique IDs per item

    // Loops through all the items in a header
    for (const auto& menu : menuData) {
        HMENU hSubMenu = CreatePopupMenu();

        // Converts json value to std::string, then to wstring
        std::string headerStr = menu["Header"].get<std::string>();
        std::wstring header(headerStr.begin(), headerStr.end());

        //Loops through all the items
        for (const auto& item : menu["Items"]) {
            std::string type = item["type"].get<std::string>();

            //If item data is "seperator", add a seperator
            if (type == "separator") {
                AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
            }
            //If Item data is "commannd", add label data 
            else if (type == "command") {
                std::string labelStr = item["label"].get<std::string>();
                std::wstring label(labelStr.begin(), labelStr.end());
                AppendMenu(hSubMenu, MF_STRING, commandId++, label.c_str());
            }
        }

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, header.c_str());
    }

    SetMenu(hwnd, hMenu);
}