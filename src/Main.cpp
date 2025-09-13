#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#define GLFW_EXPOSE_NATIVE_WIN32

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "VoxlEngine.h"
#include "Tree.h"
#include "Block.h"
#include "WorldStructure.h"
#include "FBO.h"
#include "MenuBar.h"
#include "UserInterface.h"
#include "UI_Engine.h"
#include "UI_ProjectsPanel.h"
#include "Terrain.h"
#include <core.h>

//Its cleaner if I just put this inside Camera.h
float FOV = 70.0f;

bool VoxlEngine::engineClosed = false;
bool VoxlEngine::showEngineMetrics = false;

VoxlEngine::TabModes VoxlEngine::currentTabMode = VoxlEngine::World;

std::string VoxlEngine::ConsoleText = "Voxl Engine 2025 v0.1.2.stable.alpha.52861 \nOpenGL API #330 \nC:/dev/VoxlEngine/newproject/\n";

VoxlEngine::programModes VoxlEngine::currentProgramMode = VoxlEngine::ProjectMenu;



VoxlEngine voxlEngine;
MenuBar menuBar;


GLFWwindow* CreateProjectsWindow() {
    GLFWwindow* win = glfwCreateWindow(1280, 720, "Voxl Projects", nullptr, nullptr);
    if (!win) return nullptr;

    glfwMakeContextCurrent(win);
    gladLoadGL();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return win;
}


GLFWwindow* CreateEngineWindow(int width, int height) {
    GLFWwindow* win = glfwCreateWindow(width, height, "Voxl Engine", NULL, NULL);
    if (!win) return nullptr;

    glfwMakeContextCurrent(win);
    gladLoadGL();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return win;
}

void RunProjectMenu(GLFWwindow* window)
{
    //Sets the window icon
    int iconWidth, iconHeight, iconChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* iconPixels = stbi_load("C:/dev/Voxl-Engine/Images/Branding/TerracubeGradient_Small.png", &iconWidth, &iconHeight, &iconChannels, 4);
    GLFWimage images[1];
    images[0].width = iconWidth;
    images[0].height = iconHeight;
    images[0].pixels = iconPixels;
    glfwSetWindowIcon(window, 1, images);

    Font::InitializeAllFonts();


    EngineUI_Class::ApplyDarkTheme(EngineUI_Class::UI_TYPE::Tabs);


    ProjectsPanel::CreateMainProjectsMenu(window);
}

void RunProject(Shader shaderProgram, Shader screenShader, FBO sceneFBO, VAO cubeVAO, VAO quadVAO, int width, int height, GLFWwindow* window, Camera &camera)
{
    voxlEngine.CalculateFPS();
    float currentTime = (float)glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activates the main shader
    shaderProgram.Activate();

    sceneFBO.Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // sunlight 
    glm::vec3 sunDir = glm::normalize(glm::vec3(currentTime, -1.0f, -0.2f));
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "sunDirection"), sunDir.x, sunDir.y, sunDir.z);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "sunColor"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "sunStrength"), 0.8f);

    //Camera Properties
    camera.Inputs(window);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    camera.Matrix(FOV, 0.1f, 250.0f, shaderProgram, "camMatrix", width, height);

    glUniform1f(glGetUniformLocation(shaderProgram.ID, "time"), currentTime);

    // Bind cube VAO so SpawnBlock can draw
    cubeVAO.Bind();

    // Spawn chunks
    Chunk::SpawnChunks(0, shaderProgram);

    cubeVAO.Unbind();
    sceneFBO.Unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    screenShader.Activate();
    if (screenShader.VertexShaderName == "Shaders/pixelation.vert")
    {
        screenShader.setFloat("pixelSize", 1.0f);
        screenShader.setVec2("resolution", (float)width, (float)height);
    }
    glBindTexture(GL_TEXTURE_2D, sceneFBO.textureID);
    quadVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    quadVAO.Unbind();


    glm::vec3 CurrentChunkPosition;

    CurrentChunkPosition.x = round(camera.Position.x / 32.0f);
    CurrentChunkPosition.y = 1.0f;
    CurrentChunkPosition.z = round(camera.Position.z / 32.0f);



    //Only renders Engine metrics if user clicks the button on the navbar
    if (VoxlEngine::showEngineMetrics)
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
        Canvas DeveloperMenu(ImVec2(50, 700), ImVec2(800, 800), "DEVELOPER MENU");
        TextBlock txt_FPS(DeveloperMenu, fmt::format("FPS: {:1}", round(VoxlEngine::FPS)));
        TextBlock txt_DeltaTime(DeveloperMenu, fmt::format("Delta Time: {:.10f}", VoxlEngine::DTPS));
        TextBlock txt_RunTime(DeveloperMenu, fmt::format("Runtime: {:.1f} seconds", currentTime));
        TextBlock txt_buffer01(DeveloperMenu, " ");
        TextBlock txt_CameraSpeed(DeveloperMenu, fmt::format("Camera Speed {:.1f}", camera.speed));
        TextBlock txt_CameraRotation(DeveloperMenu, fmt::format("Camera Rotation {:.1f}, {:.1f}, {:.1f}", camera.Orientation.x, camera.Orientation.y, camera.Orientation.z));
        TextBlock txt_CameraPosition(DeveloperMenu, fmt::format("Camera Position {:.1f}, {:.1f}, {:.1f}", camera.Position.x, camera.Position.y, camera.Position.z));
        TextBlock txt_buffer02(DeveloperMenu, " ");
        TextBlock txt_ChunkPosition(DeveloperMenu, fmt::format("Chunk: {:.1f}, {:.1f}, {:.1f}", CurrentChunkPosition.x, CurrentChunkPosition.y, CurrentChunkPosition.z));
        TextBlock txt_ChunkIndex(DeveloperMenu, fmt::format("Chunk: {:1}", Chunk::returnChunk(CurrentChunkPosition)));
        TextBlock txt_buffer03(DeveloperMenu, " ");
        TextBlock txt_cameraMode(DeveloperMenu, fmt::format("[L-Alt] Camera Free: {:s}", camera.ImitatePlayer ? "FALSE" : "TRUE"));
        TextBlock txt_resolution(DeveloperMenu, fmt::format("Display Resolution: {:1}/{:1}", voxlEngine.getDisplayResolution().x, voxlEngine.getDisplayResolution().y));
        Canvas::Render(DeveloperMenu);
        ImGui::PopStyleColor();
    }

    //Default Engine UI
    EngineUI_Defaults::MenuBar();
    EngineUI_Defaults::TabBar();
    EngineUI_Defaults::ProjectName();
    EngineUI_Defaults::ObjectProperties();
    EngineUI_Defaults::Console();
    EngineUI_Defaults::ProjectFiles();

    switch (VoxlEngine::currentTabMode)
    {
    case (VoxlEngine::World):
        EngineUI_Defaults::SceneCollection();
        EngineUI_Defaults::ToolBar_World();
        break;
    case (VoxlEngine::TextureEditor):
        EngineUI_Defaults::TexturePanel();
        break;
    }
}


int main()
{
    //OPENGL Initializations
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    //NEW PROJECTS WINDOW
    GLFWwindow* projectsWindow = CreateProjectsWindow();
    if (!projectsWindow) {
        std::cerr << "Failed to create projects window\n";
        glfwTerminate();
        return -1;
    }
    std::string chosenProjectName;
    std::string chosenProjectPath;


    while (!glfwWindowShouldClose(projectsWindow) && VoxlEngine::currentProgramMode == VoxlEngine::ProjectMenu)
    {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(projectsWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.063f, 0.063f, 0.063f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RunProjectMenu(projectsWindow);

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(projectsWindow);

    }

    // cleanup project window 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(projectsWindow);


    //ENGINE RENDERING
    int width = VoxlEngine::windowWidth;
    int height = VoxlEngine::windowHeight;


    GLFWwindow* window = CreateEngineWindow(width, height);
    if (!window) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make context and load GL
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    //An additional Engine Optimization: Hides faces that face away from the camera (essentially only rendering a single side of a plane)
    glEnable(GL_CULL_FACE);
    // Initialize block textures 
    Block::InitTextures();


    glClearColor(0.753f, 0.847f, 1.0f, 1.0f); // Sky white
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    //Sets the window icon
    int iconWidth, iconHeight, iconChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* iconPixels = stbi_load("C:/dev/Voxl-Engine/Images/Branding/TerracubeGradient_Small.png", &iconWidth, &iconHeight, &iconChannels, 4);
    GLFWimage images[1];
    images[0].width = iconWidth;
    images[0].height = iconHeight;
    images[0].pixels = iconPixels;
    glfwSetWindowIcon(window, 1, images);

    Chunk::InitTerrainAtlas();

    // Shaders
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    Shader screenShader("Shaders/pixelation.vert", "Shaders/pixelation.frag");

    screenShader.Activate();
    screenShader.setInt("screenTexture", 0);

    shaderProgram.Activate();
    shaderProgram.setInt("tex0", 0);


    glActiveTexture(GL_TEXTURE0);

    // Cube geometry 
    GLfloat cubeVertices[] =
    {
        // FRONT
        -0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,
         0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,
         0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,
        -0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,

        // BACK
         0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,

         // LEFT
         -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
         -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
         -0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
         -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,

         // RIGHT
          0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
          0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
          0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
          0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,

          // TOP
         -0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,
          0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,
          0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,
         -0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,

         // BOTTOM
         -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
          0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
          0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
         -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f
    };

    GLuint cubeIndices[] =
    {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    // creates VAO/VBO/EBO
    VAO cubeVAO;
    cubeVAO.Bind();
    VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
    EBO cubeEBO(cubeIndices, sizeof(cubeIndices));
    cubeVAO.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    cubeVAO.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    cubeVAO.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    cubeVAO.Unbind();
    cubeVBO.Unbind();
    cubeEBO.Unbind();

    
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    VAO quadVAO;
    quadVAO.Bind();
    VBO quadVBO(quadVertices, sizeof(quadVertices));
    quadVAO.LinkAttrib(0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    quadVAO.LinkAttrib(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    quadVAO.Unbind();
    quadVBO.Unbind();

    FBO sceneFBO(width, height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Data references
    auto allBlocks = Block::blockDatabase;
    std::vector<glm::vec3> allPlacedBlocksPos;
    auto allBlockTextures = Block::allBlockTextures;
    int blockDataBaseLength = (int)allBlocks.size();
    int blockTextureBaseLength = (int)allBlockTextures.size();

    Camera camera(width, height, glm::vec3(0.0f, 24.0f, 2.0f));

    WorldStructure Ruins(std::string("Ruins"),
        {
            Block::ReturnBlock("Darkstone"),
            Block::ReturnBlock("Mossy Darkstone"),
            Block::ReturnBlock("Stone Brick"),
            Block::ReturnBlock("Mossy Stone Brick"),
            Block::ReturnBlock("Cracked Stone Brick"),
            Block::ReturnBlock("Chest")
        });

    activeCamera = &camera;
    glfwSetScrollCallback(window, scroll_callback);

    IMGUI_CHECKVERSION();


    float DevMenuFontSize = 16.0f;
    ImGuiStyle& DevMenuStyle = ImGui::GetStyle();
    DevMenuStyle.Colors[ImGuiCol_WindowBg] = ImVec4(1, 1, 1, 0);
    DevMenuStyle.Colors[ImGuiCol_TitleBg] = ImVec4(1, 1, 1, 0);
    DevMenuStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(1, 1, 1, 0);
    DevMenuStyle.Colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);

    // Fonts
    ImFont* SpaceMono_Regular = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Regular.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* SpaceMono_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* Doto = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Doto-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* Doto_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Doto-Black.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    DevMenuStyle.WindowBorderSize = 0.0f;

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);


    // Engine render loop
    while (!glfwWindowShouldClose(window) && !VoxlEngine::engineClosed && VoxlEngine::currentProgramMode == VoxlEngine::RunningProject)
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RunProject(shaderProgram, screenShader, sceneFBO, cubeVAO, quadVAO, width, height, window, camera);

        ImGui::Render();
        
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }


    // 1. delete engine GL resources first
    shaderProgram.Delete();
    cubeVBO.Delete();
    cubeVAO.Delete();
    cubeEBO.Delete();

    for (int i = 0; i < blockTextureBaseLength; i++)
    {
        allBlockTextures[i].Delete();
    }

    // 2. shutdown imgui backends
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    // 3. destroy the engine window
    glfwDestroyWindow(window);

    // 4. finally cleanup global context + glfw
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;

}
