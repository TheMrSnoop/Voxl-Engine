#include "UI_Engine.h"


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
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.251, 0.251, 0.251, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = engineColors.DarkGray;

		//Variables
		style.TabRounding = 0.0f;
	case EngineUI_Class::Button:
		style.Colors[ImGuiCol_Button] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.251, 0.251, 0.251, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f);
		//Text-Align: Left
		style.ButtonTextAlign = ImVec2(0.0f, 0.0f);

	case EngineUI_Class::TreeNode:
		style.Colors[ImGuiCol_Button] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonActive] = engineColors.DarkGray;
		style.Colors[ImGuiCol_ButtonHovered] = engineColors.DarkGray;
	}
}



//UI RENDERING


bool EngineUI_Class::CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize)
{
	ImGui::SetNextWindowPos(ImVec2(screenPos.x, screenPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y));

	return ImGui::Begin(titleBar, nullptr, flags);
}

void EngineUI_Class::CreateTab(const char* headerText, ImVec4 color)
{
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::TreeNode);

	if (ImGui::BeginTabItem(headerText))
	{
		if (headerText == "Texture Editor")
		{
			VoxlEngine::currentTabMode = VoxlEngine::TextureEditor;
		}
		else
		{
			//temporary
			VoxlEngine::currentTabMode = VoxlEngine::World;
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

void EngineUI_Class::CreateFolder(std::string folderName, std::vector<fileData> childrenData)
{
	//flags
	ImGuiTreeNodeFlags folderFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags fileFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	//icons
	GLuint img_folder = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/folder.png");
	GLuint img_code = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/code.png");
	GLuint img_texture = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ObjectIcons/texture.png");


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

		for (EngineUI_Class::fileData data : childrenData)
		{
			switch (data.type)
			{
			case EngineUI_Class::folder:
				// folder icon
				EngineUI_Class::CreateImage(img_folder, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			case EngineUI_Class::script:
				// folder icon
				EngineUI_Class::CreateImage(img_code, ImVec2(16, 16));
				ImGui::SameLine();
				break;
			case EngineUI_Class::texture:
				// folder icon
				EngineUI_Class::CreateImage(img_texture, ImVec2(16, 16));
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
		for (EngineUI_Class::ObjectPropertyData propData : properties.PropetyData)
		{
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
			//Undo the undent for the next item
			ImGui::Indent(-indentAmount);
		}
		ImGui::TreePop();
	}
}