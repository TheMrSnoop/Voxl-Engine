#include "UI_Engine.h"

Colors engineColors;


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


		ImGui::SetNextWindowPos(ImVec2(VoxlEngine::windowWidth / 2, 0), ImGuiCond_Always);

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

	EngineUI_Class::CreatePanel("##TabBar", BGP_Tabs_F, ImVec2(0, 20), ImVec2(VoxlEngine::windowWidth, 15));

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

	const GLuint img_select = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/select.png");
	const GLuint img_transform = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/Move.png");
	const GLuint img_rotate = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/rotate.png");
	const GLuint img_scale = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/scale.png");

	GLuint img_add = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/add.png");
	GLuint img_brush = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/paint.png");

	EngineUI_Class::CreatePanel("##Toolbar", BGP_Toolbar_F, ImVec2(0, 30), ImVec2(VoxlEngine::windowWidth - 250, 55));
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
	EngineUI_Class::CreatePanel("##projectName", BGP_ProjectName_F, ImVec2((VoxlEngine::windowWidth / 2) + 20, -25), ImVec2(100, 50));
	ImGui::Text("NEW PROJECT");
	//Closes Project Title
	ImGui::End();
}

void EngineUI_Defaults::SceneCollection()
{
	//Scene Panel
	//const vs constexpr
	//uh, basically constexpr allows for potential performance gains
	constexpr ImGuiWindowFlags BGP_Scene_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##SceneCollection", BGP_Scene_F, ImVec2(VoxlEngine::windowWidth - 250, 25), ImVec2(250, VoxlEngine::windowHeight / 2));
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

		const GLuint newImageData = Image::GenerateImage(buttonImagePath);

		ImGui::Image((ImTextureID)(intptr_t)newImageData, ImVec2(25, 25));

		ImGui::SameLine();

		EngineUI_Class::CreateButton(btn.text, ImVec2(250, 25));
	}

	//Closes Scene View
	ImGui::End();

}

void EngineUI_Defaults::ObjectProperties()
{
	std::vector<EngineUI_Class::ObjectProperty> VoxelLandscapeProperties = {
		{
			"Noise Generation", // Header
			{
				{"Seed", 13803812},
				{"Biome Gen", std::string("default")},
				{"Surface Gen", std::string("default")}
			}
		},
		{
			"Landscape Properties", // Header
			{
				{"Size", std::string("16x16")},
				{"Trees Per Chunk", 2},
			}
		},
	};


	//Properties Panel
	ImGuiWindowFlags BGP_Properties_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##propertiesPanel", BGP_Properties_F, ImVec2(VoxlEngine::windowWidth - 250, VoxlEngine::windowHeight / 2), ImVec2(250, VoxlEngine::windowHeight / 2));
	
	for (EngineUI_Class::ObjectProperty objProp : VoxelLandscapeProperties)
	{
		EngineUI_Class::CreateObjectProperties(objProp);
	}
	ImGui::End();
}

void EngineUI_Defaults::Console()
{
	//Console
	ImGuiWindowFlags BGP_Console_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;
	EngineUI_Class::CreatePanel("##console", BGP_Console_F, ImVec2(0, VoxlEngine::windowHeight - 200), ImVec2(VoxlEngine::windowWidth - 250, 150));
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
	EngineUI_Class::CreatePanel("##textureEditor", BGP_TextureEdit_F, ImVec2(0, 60), ImVec2(VoxlEngine::windowWidth - 250, VoxlEngine::windowHeight - 115));
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
	EngineUI_Class::CreatePanel("##projectFiles", BGP_ProjectFiles_F, ImVec2(0, 60), ImVec2(250, VoxlEngine::windowHeight - 235));
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
			newData.name = block.texturePath.erase(0, 40);
			newData.type = EngineUI_Class::texture;
			files_textures.push_back(newData);
		}
	}

	EngineUI_Class::CreateFolder("Scripts", files_scripts);

	EngineUI_Class::CreateFolder("Textures", files_textures);


	ImGui::End();
}