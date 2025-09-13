#pragma once
#include "UI_Engine.h"

class ProjectsPanel
{
public:
    struct ProjectCardProperties
    {
        float margin_left;
        float margin_top;

        float padding_left;
        ImVec2 size;
    };

    struct ProjectProperties
    {
        std::string name;
        GLuint imageData;
        std::string path;
    };
	static void CreateMainProjectsMenu(GLFWwindow* window);
};