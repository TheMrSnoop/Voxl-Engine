#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include<vector>
#include<iostream>

class Canvas; // "forward declaration" or something idk

class UI_Tools
{
public:
	ImVec4 convertToNormalized(float Red, float Green, float Blue, float Alpha);
};

class TextBlock
{
public:
	std::string text;
	float fontSize;
	ImVec4 color;

	TextBlock(Canvas& parentCanvas, const std::string& txt, float fs = 12.0f, const ImVec4 txt_color = ImVec4(0, 0, 0, 1));
};

class Canvas
{
public:
	std::vector<TextBlock*> Canvas_TextBlocks;

	const char* Name;

	Canvas(ImVec2 Position, ImVec2 Size, const char* CanvasName);

	static void Render(Canvas canvas);
};


class EngineUI
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


	//Functions
	static void ApplyDarkTheme(UI_TYPE type);

	static void CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize);

	static void CreateTab(const char* headerText);

	//NOTE: If there is no keybind, simply put #.
	static void CreateMenuBarDropdown(const char* dropdownName, std::vector<MenuBarData> data);
};