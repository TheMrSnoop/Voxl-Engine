#include "UserInterface.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"VoxlEngine.h"



ImVec4 UI_Tools::convertToNormalized(float Red, float Green, float Blue, float Alpha)
{
	//Converts an RGBA value to a normalized value (0.5, 0.5, 0.5, 1)
	float Normalized_Red = Red / 255.0f;
	float Normalized_Green = Green / 255.0f;
	float Normalized_Blue = Blue / 255.0f;
	float Normalized_Alpha = Alpha;

	return (ImVec4(Normalized_Red, Normalized_Green, Normalized_Blue, Normalized_Alpha));
}

TextBlock::TextBlock(Canvas& parentCanvas, const std::string& txt, float fs, const ImVec4 txt_color)
	: text(txt), fontSize(fs), color(txt_color)
{
	//Adds the created TextBlock to the specified canvas
	parentCanvas.Canvas_TextBlocks.push_back(this);
}


Canvas::Canvas(ImVec2 Position, ImVec2 Size, const char* CanvasName)
{
	Name = CanvasName;
	ImGui::SetNextWindowPos(Position, ImGuiCond_Always);
	ImGui::SetNextWindowSize(Size);
}

void Canvas::Render(Canvas canvas)
{
	ImGui::Begin(canvas.Name, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	//Renders all the Text
	for (TextBlock* TextObject : canvas.Canvas_TextBlocks)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, TextObject->color);
		ImGui::Text(TextObject->text.c_str());
		ImGui::PopStyleColor();
	}


	ImGui::End();

}



//ENGINE UI

void EngineUI::CreateTab(const char* headerText)
{
	if (ImGui::BeginTabItem(headerText))
	{
		//Do stuff
		ImGui::EndTabItem();
	}
}

void CreateMenuBarItems(std::vector<EngineUI::MenuBarData> data)
{
	//Loops through all the given data.
	for (EngineUI::MenuBarData menuBarData : data)
	{
		auto keybind = menuBarData.keyBind;
		//# == no keybind
		if (keybind == "#")
		{
			keybind = NULL;
		}


		if (menuBarData.itemText == "Exit Voxl")
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1, 0.357, 0.357, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			if (ImGui::MenuItem(menuBarData.itemText)) {
				VoxlEngine::engineClosed = true;
			}

			ImGui::PopStyleColor(2);
		}
		else
		{
			if (ImGui::MenuItem(menuBarData.itemText, keybind)) {
				// do action
			}
		}
	}

}


void EngineUI::CreateMenuBarDropdown(const char* dropdownName, std::vector<MenuBarData> data)
{
	if (ImGui::BeginMenu(dropdownName))
	{
		CreateMenuBarItems(data);
		ImGui::EndMenu();
	}
}