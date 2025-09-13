#include "UI_ProjectsPanel.h"

//Project Menu


void CreateProjectEntry(ProjectsPanel::ProjectProperties projectDetails, int index)
{
	const ImGuiWindowFlags BGP_Project_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoMove;


	std::string projectPanelID = "##newproject." + std::to_string(index);


    ProjectsPanel::ProjectCardProperties cardData;
    cardData.margin_top = 50.0f;
    cardData.margin_left = 50.0f;

    cardData.padding_left = 1.25f; //in percent
    cardData.size = ImVec2(256.0f, 256.0f);

	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	if (EngineUI_Class::CreatePanel(projectPanelID.c_str(), BGP_Project_F, ImVec2((cardData.size.x * (index * cardData.padding_left)) + cardData.margin_left, cardData.margin_top), cardData.size))
    {
        //Only sets the panel style
        //ImGui::PopStyleColor();

        
        ImGui::Indent(64.0f);
        EngineUI_Class::CreateImage(projectDetails.imageData, ImVec2(128, 128));
        ImGui::Indent(-64.0f);

        ImGuiStyle& style = ImGui::GetStyle();

        // Set ButtonTextAlign to center both horizontally and vertically (0.5, 0.5)

        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);

        EngineUI_Class::CreateButton(projectDetails.name.c_str(), ImVec2(256.0f, 50.0f));

        //project path
        ImGui::PushFont(EngineFonts::LexendLight_12px);
        EngineUI_Class::CreateButton(projectDetails.path.c_str(), ImVec2(256.0f, 25.0f));
        ImGui::PopFont();


	    std::string projectSettingsID = "##newprojectSettings." + std::to_string(index);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15, 0.15, 0.15, 1.0f));
        if (EngineUI_Class::CreatePanel(projectSettingsID.c_str(), BGP_Project_F, ImVec2((cardData.size.x * (index * cardData.padding_left)) + cardData.margin_left, cardData.margin_top + 256.0f), ImVec2(cardData.size.x, 150.0f)))
        {
            ImGui::PopStyleColor();

            style.Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.251, 0.251, 0.251, 1.0f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f);



            //Project Settings
            GLuint editIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ProjectPanel/edit.png");
            EngineUI_Class::CreateImage(editIcon, ImVec2(32, 32));
            ImGui::SameLine();
            EngineUI_Class::CreateButton("EDIT", ImVec2(256.0f - 32.0f, 50.0f));

            //sets the delete button to red
            style.Colors[ImGuiCol_ButtonHovered] = engineColors.LightRed;
            GLuint deleteIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ProjectPanel/delete.png");
            EngineUI_Class::CreateImage(deleteIcon, ImVec2(32, 32));
            ImGui::SameLine();
            EngineUI_Class::CreateButton("DELETE", ImVec2(256.0f - 32.0f, 50.0f));
        }
        ImGui::End();

        EngineUI_Class::ApplyDarkTheme(EngineUI_Class::Button);  //resets the style for the next slot


    }
	ImGui::End();
}


void ProjectsPanel::CreateMainProjectsMenu(GLFWwindow* projectsWindow)
{
	const ImGuiWindowFlags BGP_Projects_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove;

	//int w, h;
	//glfwGetFramebufferSize(projectsWindow, &w, &h);


	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	if (EngineUI_Class::CreatePanel("##tabsBar", BGP_Projects_F, ImVec2(0.0f, -25.0f), ImVec2(1280.0f, 50.0f)))
    {
        if (ImGui::BeginTabBar("ProjectBar")) {
                ImGui::PushFont(EngineFonts::LexendLight_18px);

                EngineUI_Class::CreateTab("LOCAL PROJECTS", engineColors.DarkGray);
                EngineUI_Class::CreateTab("TEMPLATES", engineColors.DarkGray);
                EngineUI_Class::CreateTab("ENGINE SETTINGS", engineColors.DarkGray);

                ImGui::PopFont();

                ImGui::EndTabBar();
        }
        ImGui::PopStyleColor();
    }
    ImGui::End();

    const GLuint terravoxIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/Grass.png");
    const GLuint minecraftIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/minecraft.png");

    std::vector<ProjectsPanel::ProjectProperties> projects
    {
        {"Terravox", terravoxIcon, "C:/Users/OneDrive/Desktop/VoxlEngineProjects/TerraVox"},
        {"Minecraft", minecraftIcon, "C:/Users/OneDrive/Desktop/VoxlEngineProjects/Minecraft"}
    };

    for (int i = 0; i < projects.size(); i++)
    {
        ImGui::PushFont(EngineFonts::LexendLight_21px);

        CreateProjectEntry(projects[i], i);

        ImGui::PopFont();
    }
	
}