#include <UI/UI_Engine.h>
#include <UserInterface.h>
#include <string>


bool FontsInitalized = false;
std::string lineText = "";


void GetLines(std::string dir)
{
    std::ifstream file(dir);
    if (!file.is_open()) {
        std::cerr << "could not open file: " << dir << std::endl;
        return;
    }

    int lines = 0;
    std::string temp;
    while (std::getline(file, temp)) {
        ++lines;
    }

    for (int i = 1; i <= lines; i++)
    {
        std::string lineSTR = std::to_string(i);
        lineText += lineSTR;
        lineText += "\n";
    }

    ImGui::Text(lineText.c_str());
    ImGui::NewLine();

    lineText = "";
}

void CreateNavbarButton(std::string fileDir)
{
    ImGui::PushStyleColor(ImGuiCol_Tab, engineColors.DarkGray);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, engineColors.DarkGray);
    ImGui::PushStyleColor(ImGuiCol_TabActive, engineColors.Gray);

    const char * LabelText = fileDir.c_str();
    ImVec2 textSize = ImGui::CalcTextSize(LabelText);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(textSize.x * 0.65, 5)); // padding
    std::string tabID = "##" + fileDir;
    bool tabOpened = ImGui::BeginTabItem(tabID.c_str());

    ImVec4 stripColor = engineColors.White;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    //Colored Strip
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    min.y = max.y - 3.0f; // 3px tall strip

    draw_list->AddRectFilled(min, max, IM_COL32(stripColor.x * 255, stripColor.y * 255, stripColor.z * 255, stripColor.w * 255));


    //Inline Icon
    GLuint fileIcon = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/TabBar/file.png");
    ImVec2 tabMin = ImGui::GetItemRectMin();
    ImVec2 tabMax = ImGui::GetItemRectMax();

    ImVec2 iconPos(tabMin.x + 8, tabMin.y + (tabMax.y - tabMin.y - 12) * 0.5f);
    draw_list->AddImage((ImTextureID)(intptr_t)fileIcon, iconPos, ImVec2(iconPos.x + 12, iconPos.y + 12));

    //Text
    ImVec2 textPos(iconPos.x + 20, tabMin.y + (tabMax.y - tabMin.y - textSize.y) * 0.5f);
    draw_list->AddText(textPos, IM_COL32(255,255,255,255), LabelText);
    

    if (tabOpened)
    {
        ImGui::EndTabItem();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}


void ScriptEditor::ScriptsBar(std::vector<std::string> OpenedScripts)
{
    if (ImGui::BeginTabBar("scriptBar"))
    {
        for (auto Script : OpenedScripts)
        {
            CreateNavbarButton(Script);
        }
		ImGui::EndTabBar();
    }
    
}

void ScriptEditor::MainWindow(std::string dir)
{
    Font::InitializeAllFonts();
        
	//Pre Configuring the Panel
	ImGuiWindowFlags BGP_CenterWindow_F =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar;

	ImGuiWindowFlags F_ScriptEditor =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar;



    if (EngineUI_Class::CreatePanel("##scriptEditorPanel", BGP_CenterWindow_F, ImVec2(250, 100), ImVec2(VoxlEngine::windowWidth * 0.75, VoxlEngine::windowHeight - 275)))
    {

        if (EngineUI_Class::CreatePanel("##scriptOptions", BGP_CenterWindow_F, ImVec2(0, 50), ImVec2(250, 25)))
        {
            if (ImGui::Button("   Compile", ImVec2(100, 50))) {
                // clicked
            }

            // get rect of the button
            ImVec2 min = ImGui::GetItemRectMin();
            ImVec2 max = ImGui::GetItemRectMax();

            // draw the icon inside the button
            ImDrawList* dl = ImGui::GetWindowDrawList();
            GLuint icon_Compile = Image::GenerateImage("C:/dev/Voxl-Engine/Images/UI_Icons/ToolBar/compile.png");
            ImVec2 iconPos(min.x + 6, min.y + (max.y - min.y - 16) * 0.5f);
            dl->AddImage((ImTextureID)(intptr_t)icon_Compile, iconPos, ImVec2(iconPos.x + 16, iconPos.y + 16));
        }
        ImGui::End();

        float codeFontSize = 18.0f;
        if (EngineUI_Class::CreatePanel("##codeLinePanel", BGP_CenterWindow_F, ImVec2(250, 82), ImVec2(50, VoxlEngine::windowHeight - 275)))
        {
            ImGui::PushFont(Font::LoadFont("SourceCodePro-Medium.ttf", codeFontSize));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.25f));
            
            GetLines(dir);

            ImGui::PopFont();
            ImGui::PopStyleColor();
        }
        ImGui::End();

        if (EngineUI_Class::CreatePanel("##codePanel", F_ScriptEditor, ImVec2(300, 82), ImVec2((VoxlEngine::windowWidth - 550), VoxlEngine::windowHeight - 275)))
        {
            ImGui::PushFont(Font::LoadFont("SourceCodePro-Medium.ttf", codeFontSize));
            std::string codeContents = VoxlEngine::returnLuaFile(dir);
            ImGui::Text(codeContents.c_str());
            ImGui::PopFont();
        }
        ImGui::End();


        if (EngineUI_Class::CreatePanel("##openedScriptsPanel", BGP_CenterWindow_F, ImVec2(250, 50), ImVec2(VoxlEngine::windowWidth - 500, 20)))
        {
            std::vector<std::string> openedFiles = {
                "PlayerMovement.lua",
                "MasterShader.cpp",
                "Enemy_AI.lua",
                "Camera.cpp"
            };

            ScriptsBar(openedFiles);
        }
        ImGui::End();
    }
    ImGui::End();
}