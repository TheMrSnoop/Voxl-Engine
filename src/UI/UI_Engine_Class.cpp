#include "UI_Engine.h"
#include <iostream>
#include <string>

//STYLE
void EngineUI_Class::ApplyDarkTheme(UI_TYPE type)
{
	ImGuiStyle& style = ImGui::GetStyle();

	//Text Color is basically shared by all the types.
	style.Colors[ImGuiCol_Text] = engineColors.White;

	//Applying specific, custom color settings.
	switch (type)
	{
	case EngineUI_Class::MenuBar:
		style.Colors[ImGuiCol_MenuBarBg] = engineColors.DarkGray;
		style.Colors[ImGuiCol_PopupBg] = engineColors.DarkGray;
		style.Colors[ImGuiCol_HeaderHovered] = engineColors.FernGreen;
	case EngineUI_Class::Tabs:
		//Colors
		style.Colors[ImGuiCol_Tab] = engineColors.DarkGray;
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f);
		style.Colors[ImGuiCol_TabActive] = engineColors.Gray;
		style.Colors[ImGuiCol_WindowBg] = engineColors.DarkGray;

		//Variables
		style.TabRounding = 0.0f;
	case EngineUI_Class::Button:
		style.Colors[ImGuiCol_Button] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonActive] = engineColors.Gray;
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f);
		//Text-Align: Left
		style.ButtonTextAlign = ImVec2(0.0f, 0.0f);

	case EngineUI_Class::TreeNode:
		style.Colors[ImGuiCol_Button] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonActive] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonHovered] = engineColors.DarkGray;


	case EngineUI_Class::Dropdown:
		style.Colors[ImGuiCol_FrameBg] = engineColors.DarkGray; 
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f); 
		style.Colors[ImGuiCol_FrameBgActive] = engineColors.DarkGray; // clicked

		//Removing the default blue
		style.Colors[ImGuiCol_Header] = engineColors.DarkGray;
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f,0.20f,0.20f,1.0f);
		style.Colors[ImGuiCol_HeaderActive] = engineColors.Gray;
	}
}




//UI RENDERING


bool EngineUI_Class::CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize)
{
	ImGui::SetNextWindowPos(ImVec2(screenPos.x, screenPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y));

	return ImGui::Begin(titleBar, nullptr, flags);
}

void EngineUI_Class::CreateTab(const char* headerText, ImVec4 color, int TabID)
{
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::TreeNode);

	if (ImGui::BeginTabItem(headerText))
	{
		switch (TabID)
		{
		case 0:
			VoxlEngine::currentTabMode = VoxlEngine::World;
			break;
		case 1:
			VoxlEngine::currentTabMode = VoxlEngine::ScriptEditor;
			break;
		case 2:
			VoxlEngine::currentTabMode = VoxlEngine::TextureEditor;
			break;
		default:
			break;
		}


		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();

		// shrink max Y to just a strip
		max.y = min.y + 3.0f; // 3px tall strip

		ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255));


		ImGui::EndTabItem();
	}
}

void EngineUI_Class::CreateButton(const char* buttonText, ImVec2 size)
{
	if (ImGui::Button(buttonText, size))
	{
		if (buttonText == "TerraVox")
		{
			VoxlEngine::currentProgramMode = VoxlEngine::RunningProject;
		}
	}
}

void CreateMenuBarItems(std::vector<EngineUI_Class::MenuBarData> data)
{
	//Loops through all the given data.
	for (EngineUI_Class::MenuBarData menuBarData : data)
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
		else if (menuBarData.itemText == "Engine Metrics")
		{
			if (ImGui::MenuItem(menuBarData.itemText)) {
				VoxlEngine::showEngineMetrics = !VoxlEngine::showEngineMetrics;
			}
		}
		else
		{
			if (ImGui::MenuItem(menuBarData.itemText, keybind)) {
				// do action
			}
		}
	}

}


void EngineUI_Class::CreateMenuBarDropdown(const char* dropdownName, std::vector<MenuBarData> data)
{
	if (ImGui::BeginMenu(dropdownName))
	{
		CreateMenuBarItems(data);
		ImGui::EndMenu();
	}
}


void EngineUI_Class::CreateImage(GLuint imageData, ImVec2 imageSize)
{
	ImGui::Image((ImTextureID)(intptr_t)imageData, imageSize);
}

void EngineUI_Class::CreateFolder(std::string folderName, std::vector<EngineFile::Data> childrenData)
{
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::TreeNode);
	//flags
	ImGuiTreeNodeFlags folderFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags fileFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	//icons
	const GLuint img_folder = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/folder-outlined.png");
	const GLuint img_code = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/code.png");
	const GLuint img_texture = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/texture.png");
	const GLuint img_mesh = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/cube.png");


	std::string folderNodeName = "##xx" + folderName;

	std::string nodeName;

	//Creates the tree
	bool open = ImGui::TreeNodeEx(folderNodeName.c_str(), folderFlags);

	ImGui::SameLine();
	// folder icon
	EngineUI_Class::CreateImage(img_folder, ImVec2(16, 16));
	ImGui::SameLine();
	// folder text
	ImGui::TextUnformatted(folderName.c_str());

	if (open)
	{
		// pushes the nodes 20px to the right
		ImGui::Indent(20.0f);

		for (EngineFile::Data data : childrenData)
		{
			switch (data.type)
			{
			case EngineFile::folder:
				// folder icon
				EngineUI_Class::CreateImage(img_folder, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			case EngineFile::script:
				// folder icon
				EngineUI_Class::CreateImage(img_code, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			case EngineFile::texture:
				// folder icon
				EngineUI_Class::CreateImage(img_texture, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			case EngineFile::mesh:
				// folder icon
				EngineUI_Class::CreateImage(img_mesh, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			}

			nodeName = data.name;

			ImGui::Selectable(nodeName.c_str());
		}
		ImGui::Indent(-20.0f);
		ImGui::TreePop();
	}
}


void EngineUI_Class::CreateObjectProperties(EngineUI_Class::ObjectProperty properties)
{
	const char* Header = properties.Header.c_str();
	
	//Creates the tree
	bool open = ImGui::TreeNodeEx(Header, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen);

	float indentAmount = 115.0f;

	if (open)
	{
		for (EngineUI_Class::ObjectPropertyData propData : properties.PropertyData)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5f);
			ImGui::Text(propData.DataName.c_str());
			ImGui::SameLine();

			std::string dataText;


			ImGui::Indent(indentAmount);
			if (std::holds_alternative<int>(propData.Data)) {
				int val = std::get<int>(propData.Data);
				dataText = std::to_string(val);
			}
			else if (std::holds_alternative<std::string>(propData.Data)) {
				std::string val = std::get<std::string>(propData.Data);
				char buf[128];
				strncpy_s(buf, val.c_str(), sizeof(buf));
				buf[sizeof(buf) - 1] = '\0';

				if (ImGui::InputText(("##" + propData.DataName).c_str(), buf, sizeof(buf)))
				{
					propData.Data = std::string(buf); // save back into variant
				}
			}


			//After the data text has been created,
			//Undo the indent for the next item
			ImGui::Indent(-indentAmount);
			ImGui::PopStyleVar();
		}
		ImGui::TreePop();
	}
}

void EngineUI_Class::CenteredText(const char* text)
{
    float windowWidth = ImGui::GetWindowSize().x;

    float textWidth = ImGui::CalcTextSize(text).x;

    // Calculates the indentation needed to center the text
    float text_indentation = (windowWidth - textWidth) * 0.5f;

    ImGui::SetCursorPosX(text_indentation);

    ImGui::Text(text);
}

void EngineUI_Class::CreateCenterWindow(std::string Title, std::string Details, std::string ConfirmText, std::string DenyText)
{
	//Pre Configuring the Panel
	ImGuiWindowFlags BGP_CenterWindow_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize;
	//Creating the panel

	float panelWidth = 500;
	float panelHeight = 350;

	if (EngineUI_Class::CreatePanel("##centerWindow", BGP_CenterWindow_F, ImVec2((VoxlEngine::windowWidth * 0.5) - (panelWidth / 2), (VoxlEngine::windowHeight * 0.5) - (panelHeight / 2)), ImVec2(panelWidth, panelHeight)))
	{
		EngineUI_Class::CenteredText(Title.c_str());
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, engineColors.LightRed);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, engineColors.LightRed);
		EngineUI_Class::CreateButton("X", ImVec2(20, 20));
		ImGui::PopStyleColor(2);
	}
	ImGui::End();
}


void Dropbox::updateStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();


	style.Colors[ImGuiCol_Text] = engineColors.White;

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.125f,0.125f,0.125f,1.0f); 
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f); 
	style.Colors[ImGuiCol_FrameBgActive] = engineColors.DarkGray; // clicked

}

void Dropbox::updateIndex(int newIndex)
{
	defaultIndex = newIndex;
}
																															//This is called an "initializer list"
Dropbox::Dropbox(std::string arg_Label, std::string arg_id, int arg_defaultIndex, std::vector<const char*> arg_items) : Label(arg_Label), defaultIndex(arg_defaultIndex), items(arg_items)
{
	updateStyle();

	if (Label != "")
	{
		ImGui::Text(Label.c_str());
		ImGui::SameLine();
	}

	//Fills out normal array to use for ImGui
	const char* dropBoxItems[items.size()];

	for (int i = 0; i < items.size(); i++)
	{
		dropBoxItems[i] = items[i];
	}

	ImGui::Combo(arg_id.c_str(), &defaultIndex, dropBoxItems, IM_ARRAYSIZE(dropBoxItems));
}