#include "UI_Engine.h"
#include "UserInterface.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"VoxlEngine.h"
#include"Texture.h"
#include"Block.h"




//STYLE

Colors engineColors;

int width = VoxlEngine::windowWidth;
int height = VoxlEngine::windowHeight;

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
	}
}



//UI RENDERING


void EngineUI_Class::CreatePanel(const char* titleBar, ImGuiWindowFlags flags, ImVec2 screenPos, ImVec2 screenSize)
{
	ImGui::SetNextWindowPos(ImVec2(screenPos.x, screenPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y));

	ImGui::Begin(titleBar, nullptr, flags);
}

void EngineUI_Class::CreateTab(const char* headerText, ImVec4 color)
{
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
		//Do stuff
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

			nodeName = data.type;

			VoxlEngine::Print("Adding: " + nodeName);

			ImGui::Selectable(data.name);
		}
		ImGui::TreePop();
	}
}







//ENGINE UI

void EngineUI_Defaults::MenuBar()
{
	//MENUBAR
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::MenuBar);
	if (ImGui::BeginMainMenuBar())
	{
		//FILE
		std::vector<EngineUI_Class::MenuBarData> fileItems =
		{
			{"New", "Ctrl+N"},
			{"Open...", "Ctrl+O"},
			{"Save Project...", "Ctrl+S"},
			{"Save Project As...", "Ctrl+Shift+S"},
			{"Exit Voxl", "#"}
		};
		EngineUI_Class::CreateMenuBarDropdown("File", fileItems);

		//EDIT
		std::vector<EngineUI_Class::MenuBarData> editItems =
		{
			{"Project Settings", "#"},
			{"Editor Settings", "#"}
		};
		EngineUI_Class::CreateMenuBarDropdown("Edit", editItems);

		//VIEW
		std::vector<EngineUI_Class::MenuBarData> viewItems =
		{
			{"Viewmode", "#"},
			{"Camera FOV", "#"},
			{"Engine Metrics", "#"}
		};
		EngineUI_Class::CreateMenuBarDropdown("View", viewItems);

		//HELP
		std::vector<EngineUI_Class::MenuBarData> helpItems =
		{
			{"Documentation", "#"},
			{"Created by TheMrSnoop", "#"},
		};
		EngineUI_Class::CreateMenuBarDropdown("Help", helpItems);


		ImGui::SetNextWindowPos(ImVec2(width / 2, 0), ImGuiCond_Always);

		ImGui::EndMainMenuBar();
	}
}

void EngineUI_Defaults::TabBar()
{
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::Tabs);
	ImGuiWindowFlags BGP_Tabs_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar;

	EngineUI_Class::CreatePanel("##TabBar", BGP_Tabs_F, ImVec2(0, 20), ImVec2(width, 15));

	if (ImGui::BeginTabBar("Bar")) {
		EngineUI_Class::CreateTab("New Scene", engineColors.Orange);
		EngineUI_Class::CreateTab("grassblock.png", engineColors.LightRed);
		EngineUI_Class::CreateTab("Campfire", engineColors.LightBlue);
		EngineUI_Class::CreateTab("Character", engineColors.LightPurple);

		ImGui::EndTabBar();
	}
	//Remember, ImGui::End(), CLOSES panels, I must fill them first though.
	ImGui::End();
}


void EngineUI_Defaults::ToolBar_World()
{
	//Toolbar
	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::Button);
	ImGuiWindowFlags BGP_Toolbar_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar;

	GLuint img_select = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/select.png");
	GLuint img_transform = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/Move.png");
	GLuint img_rotate = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/rotate.png");
	GLuint img_scale = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/scale.png");

	GLuint img_add = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/add.png");
	GLuint img_brush = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/paint.png");

	EngineUI_Class::CreatePanel("##Toolbar", BGP_Toolbar_F, ImVec2(0, 30), ImVec2(width - 250, 55));
	ImGui::SameLine();
	if (ImGui::ImageButton("btn_select", img_select, ImVec2(16, 16))) {
		// clicked
	}
	ImGui::SameLine();
	if (ImGui::ImageButton("btn_transform", img_transform, ImVec2(16, 16))) {
		// clicked
	}
	ImGui::SameLine();
	if (ImGui::ImageButton("btn_rotate", img_rotate, ImVec2(16, 16))) {
		// clicked
	}
	ImGui::SameLine();
	if (ImGui::ImageButton("btn_scale", img_scale, ImVec2(16, 16))) {
		// clicked
	}

	//Closes Toolbar
	ImGui::End();
}

void EngineUI_Defaults::ProjectName()
{
	//Project Title
	ImGuiWindowFlags BGP_ProjectName_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar;
	EngineUI_Class::CreatePanel("##projectName", BGP_ProjectName_F, ImVec2((width / 2) + 20, -25), ImVec2(100, 50));
	ImGui::Text("NEW PROJECT");
	//Closes Project Title
	ImGui::End();
}

void EngineUI_Defaults::SceneCollection()
{
	//Scene Panel
	ImGuiWindowFlags BGP_Scene_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##SceneCollection", BGP_Scene_F, ImVec2(width - 250, 25), ImVec2(250, height / 2));
	ImGui::Text("SCENE COLLECTION");
	//Buttons

	std::vector<EngineUI_Class::buttonData> sceneButtons =
	{
		{ "Voxel Landscape",  EngineUI_Class::block },
		{ "Directional Lighting", EngineUI_Class::sun },
		{ "Player", EngineUI_Class::block },
		{ "Master Shader", EngineUI_Class::block },
		{ "Voxel Foliage", EngineUI_Class::block },
		{ "Volumetric Clouds", EngineUI_Class::cloud },
		{ "Day Cycle Manager", EngineUI_Class::block },
		{ "Weather Manager", EngineUI_Class::cloud },
		{ "Season Manager", EngineUI_Class::block },
		{ "Biome Manager", EngineUI_Class::block },
		{ "World Settings", EngineUI_Class::block }
	};

	const ImVec2 sceneButtonSize = ImVec2(250, 16);
	const char* buttonImagePath;
	const char* buttonID;

	for (EngineUI_Class::buttonData btn : sceneButtons)
	{
		switch (btn.icon)
		{
		case EngineUI_Class::block:
			buttonImagePath = "C:/dev/Voxl-Engine/Images/UI_Icons/16x16/cube.png";
			break;
		case EngineUI_Class::sun:
			buttonImagePath = "C:/dev/Voxl-Engine/Images/UI_Icons/16x16/sun.png";
			break;
		case EngineUI_Class::cloud:
			buttonImagePath = "C:/dev/Voxl-Engine/Images/UI_Icons/16x16/cloud.png";
			break;
		default:
			buttonImagePath = "C:/dev/Voxl-Engine/Images/UI_Icons/16x16/cube.png";
			break;
		}

		GLuint newImageData;

		newImageData = Image::GenerateImage(buttonImagePath);

		ImGui::Image((ImTextureID)(intptr_t)newImageData, ImVec2(25, 25));

		ImGui::SameLine();

		EngineUI_Class::CreateButton(btn.text, ImVec2(250, 25));
	}

	//Closes Scene View
	ImGui::End();

}

void EngineUI_Defaults::ObjectProperties()
{
	//Properties Panel
	ImGuiWindowFlags BGP_Properties_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##propertiesPanel", BGP_Properties_F, ImVec2(width - 250, height / 2), ImVec2(250, height / 2));
	ImGui::Text("Property Value");
	ImGui::End();
}

void EngineUI_Defaults::Console()
{
	//Console
	ImGuiWindowFlags BGP_Console_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##console", BGP_Console_F, ImVec2(0, height - 200), ImVec2(width - 250, 150));
	ImGui::Text(VoxlEngine::ConsoleText.c_str());
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f) {
		ImGui::SetScrollHereY(1.0f); // 1.0f = bottom
	}
	ImGui::End();
}

void EngineUI_Defaults::TexturePanel()
{
	//Texture Editor
	ImGuiWindowFlags BGP_TextureEdit_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##textureEditor", BGP_TextureEdit_F, ImVec2(0, 60), ImVec2(width - 250, height - 115));
	ImGui::Text("Hello World");
	ImGui::End();
}

void EngineUI_Defaults::ProjectFiles()
{
	//Texture Editor
	ImGuiWindowFlags BGP_ProjectFiles_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##projectFiles", BGP_ProjectFiles_F, ImVec2(0, 60), ImVec2(250, height - 235));
	ImGui::Text("PROJECT CONTENT");

	std::vector<EngineUI_Class::fileData> files_scripts =
	{
		{"player.lua", EngineUI_Class::script},
		{"skeleton.lua", EngineUI_Class::script},
		{"sword.lua", EngineUI_Class::script}
	};

	std::vector<EngineUI_Class::fileData> files_textures;


	if (files_textures.size() != Block::blockDatabase.size())
	{
		for (Block::BlockData block : Block::blockDatabase)
		{
			EngineUI_Class::fileData newData;
			newData.name = block.texturePath.c_str();
			newData.type = EngineUI_Class::texture;
			files_textures.push_back(newData);
		}
	}



	EngineUI_Class::CreateFolder("Scripts", files_scripts);

	EngineUI_Class::CreateFolder("Textures", files_textures);


	ImGui::End();
}