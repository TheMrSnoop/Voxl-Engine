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

class Colors
{
public:

	static ImVec4 RGB_To_Normalized(float Red, float Green, float Blue, float Alpha);

	//Const Colors
	const ImVec4 White = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	const ImVec4 Silver = ImVec4(0.66f, 0.66f, 0.66f, 1.0f); //#A9A9A9
	const ImVec4 Gray = ImVec4(0.251, 0.251, 0.251, 1.0f); //#404040
	const ImVec4 DarkGray = ImVec4(0.063f, 0.063f, 0.063f, 1.0f); //#101010
	const ImVec4 MatteBlack = ImVec4(0.0352f, 0.0352f, 0.0352f, 1.0f); //#090909
	const ImVec4 JetBlack = ImVec4(0.0313f, 0.0313f, 0.0313f, 1.0f); //#080808
	const ImVec4 Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	const ImVec4 Amethyst = ImVec4(0.62f, 0.42f, 0.78f, 1.0f); //#9E6BC7
	const ImVec4 LightBlue = ImVec4(0.278f, 0.521f, 0.761f, 1.0f); 
	const ImVec4 LightPurple = ImVec4(0.773f, 0.651f, 0.867f, 1.0f); 
	const ImVec4 FernGreen = ImVec4(0.247f, 0.471f, 0.298f, 1.0f); //#3F784C

	const ImVec4 LightRed = ImVec4(0.959f, 0.419f, 0.419f, 1.0f);
	const ImVec4 Orange = ImVec4(1.0f, 0.56f, 0.0f, 1.0f);
	const ImVec4 Teal = ImVec4(0.176f, 0.535f, 0.475f, 1.0f);
};


class Font
{
public:
	struct font {
		std::string name;
		ImFont* ImFont;
		float fontSize;
	};

	static const std::vector<const char*> AllFontsDirectories;

	const std::string EngineFontsPath = "";
	
	//inline defines AllFonts directly inside the header. (C++ 17+)
	inline static std::vector<font> AllFonts;

	static void InitializeAllFonts();

	static ImFont* CreateFont(const char* fileDir, float fontSize);
	static ImFont* LoadFont(std::string fileName, float fontSize);


	
};

class EngineFonts
{
public:
	static ImFont* LexendLight_12px;
	static ImFont* LexendLight_14px;
	static ImFont* LexendLight_16px;
	static ImFont* LexendLight_18px;
	static ImFont* LexendLight_21px;
	static ImFont* SRC_Pro_16px;
};