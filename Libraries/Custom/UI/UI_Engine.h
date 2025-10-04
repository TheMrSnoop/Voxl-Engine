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
#include<variant>

#include "UI_Engine.h"
#include "UserInterface.h"
#include"VoxlEngine.h"
#include"Texture.h"
#include"Block.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <stb/stb_image.h>

extern Colors engineColors;
extern Font engineFonts;



class EngineFile
{
public:
	enum Type
	{
		folder,
		script,
		texture,
		mesh
	};

	
	struct Data
	{
		std::string name;
		Type type;
	};


	inline static const std::vector<std::string> extensions_script =
	{
		".lua",
		".cpp",
		".h",
		".c"	
	};

	inline static const std::vector<std::string> extensions_texture = 
	{
		".png",
		".jpeg",
		".jpg"
	};

	inline static const std::vector<std::string> extensions_mesh = 
	{
		".fbx",
		".obj",
		".blend"
	};
};



class EngineUI_Class
{
public:
	//CUSTOM DATA TYPES
	//apparently I need to drop the static keyword.
	enum UI_TYPE
	{
		MenuBar,
		Tabs,
		Panel,
		Button,
		TreeNode,
		Dropdown
	};


	struct MenuBarData
	{
		const char* itemText;
		const char* keyBind;
	};


	enum iconType
	{
		block,
		cloud,
		sun
	};

	struct buttonData
	{
		const char* text;
		iconType icon;
	};





	struct ObjectPropertyData
	{
		std::string DataName;

		//Data can only either be an int or string.
		std::variant<int, std::string> Data;
	};

	struct ObjectProperty
	{
		std::string Header;
		std::vector<ObjectPropertyData> PropertyData;
	};



	//Create UI Classes
	static void ApplyDarkTheme(UI_TYPE type);

	static bool CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize);

	static void CreateTab(const char* headerText, ImVec4 color, int TabID);

	static void CreateButton(const char* buttonText, ImVec2 size);

	static void CreateImage(GLuint imageData, ImVec2 imageSize);

	//NOTE: If there is no keybind, simply put #.
	static void CreateMenuBarDropdown(const char* dropdownName, std::vector<MenuBarData> data);

	static void CreateFolder(std::string folderName, std::vector<EngineFile::Data> childrenData);

	static void CreateProjectEntry(std::string projectDetails, int index);

	static void CreateObjectProperties(EngineUI_Class::ObjectProperty properties);

	static void CreateCenterWindow(std::string Title, std::string Details, std::string ConfirmText, std::string DenyText);

	static void CenteredText(const char* text);

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


class ScriptEditor
{
public:
	static void MainWindow(std::string dir);
	static void ScriptsBar(std::vector<std::string> OpenedScripts);
};



