#include "UI_ProjectsPanel.h"


//Project Menu


void CreateProjectEntry(ProjectsPanel::ProjectProperties projectDetails, int index)
{

	const ImGuiWindowFlags BGP_ProjectsContainer_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar;


	const ImGuiWindowFlags BGP_Project_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove;



    ProjectsPanel::ProjectCardProperties cardData;
    cardData.margin_top = 50.0f;
	std::string projectPanelID = "##newproject." + std::to_string(index);

    cardData.margin_left = 50.0f;

    cardData.padding_left = 1.25f; //in percent
    cardData.size = ImVec2(256.0f, 256.0f);

    //Border Style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;

    //style.Colors[ImGuiCol_Border] = engineColors.Silver; // red border
    style.WindowBorderSize = 0.5f; // thickness > 0 enables the border

    ImVec2 CardPosition;

    if (index > 2) //if so, wrap content.
    {
        CardPosition = ImVec2((cardData.size.x * (0 * cardData.padding_left)) + cardData.margin_left, (cardData.margin_top * 2) + (cardData.size.y + 125.0f));
    } else {
        CardPosition = ImVec2((cardData.size.x * (index * cardData.padding_left)) + cardData.margin_left, cardData.margin_top);
    }
    

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.05f));
    if (EngineUI_Class::CreatePanel("##projectsContainer", BGP_ProjectsContainer_F, ImVec2(50.0f, 50.0f), ImVec2(900.0f, 720.0f)))
    {
        ImGui::PopStyleColor();
        if (EngineUI_Class::CreatePanel(projectPanelID.c_str(), BGP_Project_F, CardPosition, cardData.size))
        {
            //Only sets the panel style
            //ImGui::PopStyleColor();

            
            ImGui::Indent(64.0f);
            EngineUI_Class::CreateImage(projectDetails.imageData, ImVec2(128, 128));
            ImGui::Indent(-64.0f);

            ImGuiStyle& style = ImGui::GetStyle();

            // Set ButtonTextAlign to center both horizontally and vertically (0.5, 0.5)

            style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
            style.FrameBorderSize = 0.0f;

            EngineUI_Class::CreateButton(projectDetails.name.c_str(), ImVec2(256.0f, 50.0f));

            //project path
            ImGui::PushFont(EngineFonts::LexendLight_12px);
            EngineUI_Class::CreateButton(projectDetails.path.c_str(), ImVec2(256.0f, 25.0f));
            ImGui::PopFont();


            std::string projectSettingsID = "##newprojectSettings." + std::to_string(index);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15, 0.15, 0.15, 1.0f));
            if (EngineUI_Class::CreatePanel(projectSettingsID.c_str(), BGP_Project_F, ImVec2(CardPosition.x, CardPosition.y + 275.0f), ImVec2(cardData.size.x, 125.0f)))
            {
                ImGui::PopStyleColor();

                style.Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1.0f);
                style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.251, 0.251, 0.251, 1.0f);
                style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.369, 0.369, 0.369, 1.0f);



                //Project Settings
                const GLuint editIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ProjectPanel/edit.png");
                EngineUI_Class::CreateImage(editIcon, ImVec2(50, 50));
                ImGui::SameLine();
                EngineUI_Class::CreateButton("EDIT", ImVec2(256.0f - 50.0f, 50.0f));

                //sets the delete button to red
                //style.Colors[ImGuiCol_ButtonHovered] = engineColors.LightRed;
                //GLuint deleteIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ProjectPanel/delete.png");
                //ImGui::Indent(25/2);

                const GLuint renameIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ProjectPanel/rename.png");
                EngineUI_Class::CreateImage(renameIcon, ImVec2(50, 50));
                //ImGui::Indent(-25/2);
                ImGui::SameLine();
                EngineUI_Class::CreateButton("RENAME", ImVec2(256.0f - 50.0f, 50.0f));
            }
            ImGui::End();

            EngineUI_Class::ApplyDarkTheme(EngineUI_Class::Button);  //resets the style for the next slot


        }
        ImGui::End();
    }
    ImGui::End();
}

void CreateCenteredText(const char* text, float y)
{
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImVec2 winSize  = ImGui::GetWindowSize();

    float x = (winSize.x - textSize.x) * 0.5f;

    ImGui::SetCursorPos(ImVec2(x, y));
    ImGui::Text("%s", text);
}

void ProjectsPanel::CreateMainProjectsMenu(GLFWwindow* projectsWindow)
{
	const ImGuiWindowFlags BGP_Projects_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoMove;

	//int w, h;
	//glfwGetFramebufferSize(projectsWindow, &w, &h);

    //resets all style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f; // none


	if (EngineUI_Class::CreatePanel("##createdProjectsText", BGP_Projects_F, ImVec2(445.0f, 10.0f), ImVec2(125.0f, 25.0f)))
    {
        ImGui::Text("Created Projects");
    }
    ImGui::End();

    const GLuint terravoxIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/ProjectIcons/Grass.png");
    const GLuint minecraftIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/ProjectIcons/minecraft.png");
    const GLuint teardownIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/ProjectIcons/teardown.png");
    const GLuint terrariaIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/ProjectIcons/Terraria.png");

    std::vector<ProjectsPanel::ProjectProperties> projects
    {
        {"Terravox", terravoxIcon, "Desktop/VoxlEngineProjects/TerraVox"},
        {"Minecraft", minecraftIcon, "Desktop/VoxlEngineProjects/Minecraft"},
        {"Teardown", teardownIcon, "Desktop/VoxlEngineProjects/Teardown"},
        {"Terraria", terrariaIcon, "Desktop/VoxlEngineProjects/Terraria"}

    };

    if (EngineUI_Class::CreatePanel("##engineVersion", BGP_Projects_F, ImVec2(50.0f, 50.0f), ImVec2(50.0f, 25.0f)))
    {
        ImGui::Text("Voxl Engine 2025 v0.1.2.stable.alpha.52861");
    }
    ImGui::End();

    for (int i = 0; i < projects.size(); i++)
    {
        ImGui::PushFont(EngineFonts::LexendLight_21px);

        CreateProjectEntry(projects[i], i);

        ImGui::PopFont();
    }

    if (EngineUI_Class::CreatePanel("##CreateProjectPanel", BGP_Projects_F, ImVec2(1000.0f, 50.0f), ImVec2(250.0f, 620.0f)))
    {
        ImGui::PushFont(EngineFonts::LexendLight_21px);
        CreateCenteredText("New Project", 20.0f);
        ImGui::PopFont();


        ImGui::PushFont(EngineFonts::LexendLight_16px);

        GLuint img_thumbnail = Image::GenerateImage("C:/dev/Voxl-Engine/Images/project_thumbnail.png");
        EngineUI_Class::CreateImage(img_thumbnail, ImVec2(225.0f, 225.0f));

        CreateCenteredText("A completely empty project.", 275.0f);


        static int currentVoxelIdx = 0;
        const char* voxelSizes[] = { "1m", "0.5m", "0.25m" };

        EngineUI_Class::ApplyDarkTheme(EngineUI_Class::Dropdown);
        ImGui::Text("Voxel Size");
        ImGui::SameLine();

        float DropdownWidth = 120.0f;
        float DropdownIndent = 108.0f;

        ImGui::PushItemWidth(DropdownWidth); // set width
        ImGui::Indent(DropdownIndent);
        if (ImGui::BeginCombo("##voxelSize", voxelSizes[currentVoxelIdx])) // label, preview
        {
            for (int n = 0; n < IM_ARRAYSIZE(voxelSizes); n++)
            {
                bool isSelected = (currentVoxelIdx == n);
                if (ImGui::Selectable(voxelSizes[n], isSelected))
                    currentVoxelIdx = n;
                if (isSelected)
                    ImGui::SetItemDefaultFocus(); // highlight currently selected choice
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::Indent(-DropdownIndent);

        
        static int currentRenderIdx = 0; 
        const char* renderModes[] = { "Ray-Tracing", "Rasterization" };

        ImGui::Text("Rendering Mode");
        ImGui::SameLine();

        ImGui::PushItemWidth(DropdownWidth);
        if (ImGui::BeginCombo("##renderMode", renderModes[currentRenderIdx]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(renderModes); n++)
            {
                bool isSelected = (currentRenderIdx == n);
                if (ImGui::Selectable(renderModes[n], isSelected))
                    currentRenderIdx = n;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();


        ImGui::Text("Use C++ in Project");
        ImGui::SameLine();
        static bool useCpp = false;
        style.Colors[ImGuiCol_CheckMark] = engineColors.FernGreen; // golden check
        ImGui::Checkbox("##check_c++", &useCpp);
        ImGui::PopFont();

    }
    ImGui::End();

	
}