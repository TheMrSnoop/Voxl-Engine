#include "UI_Engine.h"

Colors engineColors;

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

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
		EngineUI_Class::CreateTab("New Scene", engineColors.Orange, 0 );
		EngineUI_Class::CreateTab("Script Editor", engineColors.Teal, 1);
		EngineUI_Class::CreateTab("Texture Editor", engineColors.LightRed, 2);
		EngineUI_Class::CreateTab("Character Creator", engineColors.LightBlue, 3);

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
	ImGui::Text("TERRAVOX");
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
		{ "Procedural Voxel Landscape",  EngineUI_Class::block },
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


EngineFile::Type extensionRecognized(string ext)
{
	for (std::string ext_script : EngineFile::extensions_script)
	{
		if (ext_script == ext)
		{
			return EngineFile::script;
		}
	}

	for (std::string ext_texture : EngineFile::extensions_texture)
	{
		if (ext_texture == ext)
		{
			return EngineFile::texture;
		}
	}

	for (std::string ext_mesh : EngineFile::extensions_mesh)
	{
		if (ext_mesh == ext)
		{
			return EngineFile::mesh;
		}
	}

	//if none were found, return script as the default
	return EngineFile::script;
}


std::vector<EngineFile::Data> GetFilesInFolder(const std::string& folder)
{
    std::vector<EngineFile::Data> out;
    if (!fs::exists(folder)) return out;

    for (auto& entry : fs::directory_iterator(folder))
    {
        if (entry.is_regular_file())
        {
			std::string ext = entry.path().extension().string(); 

            EngineFile::Data data;
            data.name = entry.path().filename().string();
            data.type = extensionRecognized(ext);
            out.push_back(data);
        }
    }
    return out;
}



std::vector<std::string> GetFoldersInRoot(const std::string& root)
{
    std::vector<std::string> out;
    if (!fs::exists(root)) return out;

    for (auto& entry : fs::directory_iterator(root))
    {
        if (entry.is_directory())
        {
            out.push_back(entry.path().filename().string()); 
        }
    }
    return out;
}


void EngineUI_Defaults::ProjectFiles()
{
	//Pre Configuring the Panel
	ImGuiWindowFlags BGP_ProjectFiles_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;


	//Applying theme style
	ImGui::PushStyleColor(ImGuiCol_Border, engineColors.Gray);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

	EngineUI_Class::ApplyDarkTheme(EngineUI_Class::TreeNode);


	//Creating the panel
	EngineUI_Class::CreatePanel("##projectFiles", BGP_ProjectFiles_F, ImVec2(0, 60), ImVec2(250, VoxlEngine::windowHeight - 235));


	//Adding Header with text: "PROJECT CONTENT"
	ImGui::Text("PROJECT CONTENT");


	//Specifying the project Directory
	const std::string projectDirectory = "C:/Users/thoma/OneDrive/Desktop/Voxl Engine Projects/TerraVox";


	//Getting all the project folder
	std::vector<std::string> ProjectFolders = GetFoldersInRoot(projectDirectory);

	for (std::string Subfolder : ProjectFolders)
	{
		EngineUI_Class::CreateFolder(Subfolder, GetFilesInFolder(projectDirectory + "/" + Subfolder));
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	ImGui::End();
}