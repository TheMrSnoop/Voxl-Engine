#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <glad/glad.h> 
#include<vector>
#include<iostream>




class EngineUI_Class
{
public:
	//CUSTOM DATA TYPES
	static enum UI_TYPE
	{
		MenuBar,
		Tabs,
		Panel,
		Button
	};


	static struct MenuBarData
	{
		const char* itemText;
		const char* keyBind;
	};


	static enum iconType
	{
		block,
		cloud,
		sun
	};

	static struct buttonData
	{
		const char* text;
		iconType icon;
	};

	static enum fileType
	{
		folder,
		script,
		texture
	};

	static struct fileData
	{
		const char* name;
		fileType type;
	};


	//Create UI Classes
	static void ApplyDarkTheme(UI_TYPE type);

	static void CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize);

	static void CreateTab(const char* headerText, ImVec4 color);

	static void CreateButton(const char* buttonText, ImVec2 size);

	static void CreateImage(GLuint imageData, ImVec2 imageSize);

	//NOTE: If there is no keybind, simply put #.
	static void CreateMenuBarDropdown(const char* dropdownName, std::vector<MenuBarData> data);

	static void CreateFolder(std::string folderName, std::vector<fileData> childrenData);




};


//Create Specific Engine UI
class EngineUI_Defaults
{
public:

	static void MenuBar();
	static void TabBar();
	static void ProjectName();

	static void ObjectProperties();
	static void Console();
	static void ProjectFiles();

	//Mode Specific
	static void ToolBar_World();
	static void SceneCollection();
	static void TexturePanel();
};