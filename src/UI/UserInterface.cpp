#include "UserInterface.h"
#include"imgui.h"
#include"VoxlEngine.h"
#include <filesystem>
#include <string>
#include <cctype> // For std::tolower

namespace fs = std::filesystem;

ImFont* EngineFonts::LexendLight_12px;
ImFont* EngineFonts::LexendLight_14px;
ImFont* EngineFonts::LexendLight_16px;
ImFont* EngineFonts::LexendLight_18px;
ImFont* EngineFonts::LexendLight_21px;
ImFont* EngineFonts::SRC_Pro_16px;

ImVec4 Colors::RGB_To_Normalized(float Red, float Green, float Blue, float Alpha)
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


bool haveFontsBeenInitalized = false;

void Font::InitializeAllFonts() {
	if (!haveFontsBeenInitalized)
	{
		ImGuiIO& io = ImGui::GetIO();
		EngineFonts::LexendLight_12px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Lexend/Lexend-Light.ttf", 12.0f);
		EngineFonts::LexendLight_14px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Lexend/Lexend-Light.ttf", 14.0f);
		EngineFonts::LexendLight_16px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Lexend/Lexend-Light.ttf", 16.0f);
		EngineFonts::LexendLight_18px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Lexend/Lexend-Light.ttf", 18.0f);
		EngineFonts::LexendLight_21px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Lexend/Lexend-Light.ttf", 21.0f);

		EngineFonts::SRC_Pro_16px = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SourceCodePro-Medium.ttf", 16.0f);
		
		std::vector<std::string> results;

		for (auto& entry : fs::recursive_directory_iterator("C:/dev/Voxl-Engine/Fonts")) {
			if (entry.is_regular_file()) {
				auto path = entry.path();
				if (path.extension() == ".ttf") {
					results.push_back(path.string());

					//Generates font sizes (variants) 8px through 32px
					for (int i = 8; i < 32; i++)
					{
						Font::font createdFont;
						createdFont.name = path.filename().string();
						createdFont.ImFont = io.Fonts->AddFontFromFileTTF(path.string().c_str(), (i * 1.0f));
						createdFont.fontSize = i * 1.0f;

						Font::AllFonts.push_back(createdFont);	

						VoxlEngine::Print("Initializing font: " + createdFont.name + "(" + std::to_string(createdFont.fontSize) + " )for the first time.");
					}	
				}
			}
		}

		// after loading fonts, rebuild atlas once
		io.Fonts->Build();
		haveFontsBeenInitalized = true;
	}
}


ImFont* Font::CreateFont(const char* fileDir, const float fontSize) {
	const ImGuiIO& io = ImGui::GetIO();
	ImFont* MainFont = io.Fonts->AddFontFromFileTTF(fileDir, fontSize);

	return MainFont;
}

ImFont* Font::LoadFont(std::string fileName, float fontSize) {
	//Finds and returns the font object
	for (Font::font fontEntry: Font::AllFonts) {
		if (fontEntry.name == fileName && fontEntry.fontSize == fontSize) {
			return fontEntry.ImFont;
		}
	}
	VoxlEngine::Print("ERROR! FONT: " + fileName + " WAS NOT FOUND.");
	return nullptr;
}





