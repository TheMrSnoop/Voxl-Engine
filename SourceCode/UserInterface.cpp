#include "UserInterface.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"



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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}