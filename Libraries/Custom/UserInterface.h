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
	const ImVec4 White = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); //#101010
	const ImVec4 Silver = ImVec4(0.66f, 0.66f, 0.66f, 1.0f); //#A9A9A9
	const ImVec4 DarkGray = ImVec4(0.063f, 0.063f, 0.063f, 1.0f); //#101010

	const ImVec4 Amethyst = ImVec4(0.62f, 0.42f, 0.78f, 1.0f); //#9E6BC7
	const ImVec4 LightBlue = ImVec4(0.278f, 0.521f, 0.761f, 1.0f); 
	const ImVec4 LightPurple = ImVec4(0.773f, 0.651f, 0.867f, 1.0f); 
	const ImVec4 FernGreen = ImVec4(0.247f, 0.471f, 0.298f, 1.0f); //#3F784C

	const ImVec4 LightRed = ImVec4(0.959f, 0.419f, 0.419f, 1.0f);
	const ImVec4 Orange = ImVec4(1.0f, 0.56f, 0.0f, 1.0f);


};