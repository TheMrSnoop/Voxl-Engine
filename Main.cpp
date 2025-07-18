#define IMGUI_IMPL_OPENGL_LOADER_GLAD


#include"imgui.h"
#include"imgui_impl_glfw.h"

#include"imgui_impl_opengl3.h"
//#include"imgui_impl_opengl3_loader.h"

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"DeltaTime.h"
#include"Tree.h"
#include"Block.h"
#include"WorldStructure.h"
#include"FBO.h"


int width = 1920;
int height = 1080;


//technicallllly, its better to include this varriable in Camera.h, and slightly remake the logic in Camera.cpp, then to store it here.
//..why?
//uh
//because
//its cleaner? idk why I wrote this

float FOV = 70.0f;



int main()
{


	//Initializes GLFW
	glfwInit();

	//Tells GLFW what version of OpenGL I am using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);



	//Defines the window object
	


	GLFWwindow* window = glfwCreateWindow(width, height, "Voxl Engine", NULL, NULL);


	//Fullscreen
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Voxl Engine", monitor, NULL);


	//CODE FOR THE TOP TOOLBAR

	//Creates the top window
	HWND hwnd = glfwGetWin32Window(window);

	HMENU hMenu = CreateMenu();


	//Creates the first dropdown menu, "File"
	HMENU hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu, MF_STRING, 1, L"Open");
	AppendMenu(hFileMenu, MF_STRING, 2, L"Save");
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hFileMenu, MF_STRING, 3, L"Exit");

	//Creates the second dropdown menu, "Edit"
	HMENU hEditMenu = CreatePopupMenu();
	AppendMenu(hEditMenu, MF_STRING, 1, L"Project Settings");
	AppendMenu(hEditMenu, MF_STRING, 2, L"Editor Settings");

	//Creates the third dropdown menu, "View"
	HMENU hViewMenu = CreatePopupMenu();
	AppendMenu(hViewMenu, MF_STRING, 1, L"Editor Viewmode");

	//Seperates the dropdown, kinda like a horizontal rule (<hr>)
	AppendMenu(hViewMenu, MF_SEPARATOR, 0, NULL);

	AppendMenu(hViewMenu, MF_STRING, 2, L"Editor Camera FOV");
	AppendMenu(hViewMenu, MF_STRING, 3, L"Viewport Resolution");

	AppendMenu(hViewMenu, MF_SEPARATOR, 0, NULL);

	AppendMenu(hViewMenu, MF_STRING, 4, L"Viewport Window");
	AppendMenu(hViewMenu, MF_STRING, 5, L"File Explorer Windows");

	//Creates the fourth dropdown menu, "Documentation"
	HMENU hDoc = CreatePopupMenu();
	AppendMenu(hDoc, MF_STRING, 1, L"Voxl Documentation");
	AppendMenu(hDoc, MF_SEPARATOR, 0, NULL);
	AppendMenu(hDoc, MF_STRING, 2, L"Created by TheMrSnoop");


	//Adds both of the dropdown menus to the window
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"View");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDoc, L"Documentation");

	SetMenu(hwnd, hMenu);

	//If window failed, create error message
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}


	GLfloat vertices[] =
	{
			//CODE FOR CUBE
			//       Cordinates      /        Colors       // Texture Cordinates
			// FRONT
			-0.5f, -0.5f, 0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
			 0.5f, -0.5f, 0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
			 0.5f,  0.5f, 0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
			-0.5f,  0.5f, 0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 1.0f,

			// BACK
			0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 1.0f,

			// LEFT
		   -0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 1.0f,

		   // RIGHT
		   0.5f, -0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
		   0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
		   0.5f,  0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 1.0f,

		   // TOP
		  -0.5f, 0.5f,  0.5f,    0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
		   0.5f, 0.5f,  0.5f,    0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
		   0.5f, 0.5f, -0.5f,    0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
		  -0.5f, 0.5f, -0.5f,    0.8f, 0.7f, 0.4f,    0.0f, 1.0f,

		  // BOTTOM
		 -0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   0.8f, 0.7f, 0.4f,    0.0f, 1.0f

		//CODE FOR PYRAMID
		//       Cordinates      /        Colors       // Texture Cordinates
		//-0.5f,  0.0f,   0.5f,      0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  
		//-0.5f,  0.0f,  -0.5f,      0.83f, 0.70f, 0.44f,    5.0f, 0.0f,  
		//0.5f,  0.0f,  -0.5f,      0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  
		//0.5f,  0.0f,   0.5f,      0.83f, 0.70f, 0.44f,    5.0f, 0.0f,   
		//0.0f,  0.8f,   0.0f,      0.92f, 0.86f, 0.76f,    2.5f, 5.0f 

		//CODE FOR SQUARE/PLANE
		//       Cordinates      /        Colors       // Texture Cordinates
		//-0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f,   //Lower left corner
		//-0.5f, 0.5f,  0.0f,      0.0f, 1.0f, 0.0f,    0.0f, 1.0f,   //Upper left corner
		//0.5f, 0.5f, 0.0f,        0.0f, 0.6f, 1.0f,    1.0f, 1.0f,   //Upper right corner
		//0.5f, -0.5f, 0.0f,       1.0f, 1.0f, 1.0f,    1.0f, 0.0f    //Lower right corner
	};

	GLuint indices[] =
	{   
		0, 1, 2,    2, 3, 0,      // front
		4, 5, 6,    6, 7, 4,      // back
		8, 9,10,   10,11, 8,      // left
	   12,13,14,   14,15,12,      // right
	   16,17,18,   18,19,16,      // top
	   20,21,22,   22,23,20       // bottom

		//INDICES FOR PYRAMID
		//0, 1, 2,
		//0, 2, 3,
		//0, 1, 4,
		//1, 2, 4,
		//2, 3, 4,
		//3, 0, 4

		//INDICES FOR SQUARE/PLANE
		//0, 2, 1, //Upper triangle
		//0, 3, 2 //Lower triangle
	};




	//Loads the Window

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	//Initalizes all the block textures AFTER gladLoadGL()
	Block::InitTextures();




	//Renders the background color, then applys it to the next frame.

	//glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // == Dark Blue
	//glClearColor(0.063f, 0.063f, 0.063f, 1.0f); //== Dark Gray
	//glClearColor(0.106f, 0.624f, 1.0f, 1.0f); //== Sky blue
	glClearColor(0.753f, 0.847f, 1.0f, 1.0f); //== Sky White

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	glDisable(GL_DEPTH_TEST);

	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glEnable(GL_TEXTURE_2D);


	//Loads icon image file
	int iconWidth, iconHeight;
	int iconChannels;


	stbi_set_flip_vertically_on_load(false);
	unsigned char* iconPixels = stbi_load("C:/dev/Voxl-Engine/Images/VoxlCube.png", &iconWidth, &iconHeight, &iconChannels, 4);


	//Changes the window icon
	GLFWimage images[1];
	images[0].width = iconWidth;
	images[0].height = iconHeight;
	images[0].pixels = iconPixels;

	glfwSetWindowIcon(window, 1, images);


	Shader shaderProgram("default.vert", "default.frag");

	Shader screenShader("pixelation.vert", "pixelation.frag");

	screenShader.Activate();
	screenShader.setInt("screenTexture", 0); // bind to texture unit 0


	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	int indexCount = sizeof(indices) / sizeof(GLuint);

	VAO1.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)(0));                  // pos
	VAO1.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // color
	VAO1.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // uv

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


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
	quadVAO.LinkAttrib(0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);           // pos
	quadVAO.LinkAttrib(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float))); // uv

	quadVAO.Unbind();
	quadVBO.Unbind();



	FBO sceneFBO(width, height);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





	//Gets a reference to the Block Database
	auto allBlocks = Block::blockDatabase;

	std::vector<glm::vec3> allPlacedBlocksPos;

	//Gets a reference to all the Block Textures
	auto allBlockTextures = Block::allBlockTextures;

	//Gets both of the array lengths
	int blockDataBaseLength = allBlocks.size();
	int blockTextureBaseLength = allBlocks.size();


	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 2.0f));

	DeltaTime deltatime(0.5f);

	//Trees
	Tree OakTree(std::string("Oak"), Block::ReturnBlock("Log"), Block::ReturnBlock("Leaves"));
	Tree PineTree(std::string("Pine"), Block::ReturnBlock("Pine Log"), Block::ReturnBlock("Pine Leaves"));
	Tree LargeOakTree(std::string("Oak_Large"), Block::ReturnBlock("Log"), Block::ReturnBlock("Leaves"));
	Tree FallenLog(std::string("Fallen_Log"), Block::ReturnBlock("Log"), Block::ReturnBlock("Leaves"));
	Tree Rock(std::string("Stone_Rock"), Block::ReturnBlock("Stone"), Block::ReturnBlock("Leaves"));


	//Tree::fillOutRandomTreePositions(glm::vec2(-10, 10), glm::vec2(-10, 10), 0.0f, 10);


	WorldStructure Ruins(std::string("Ruins"), 
	{
		Block::ReturnBlock("Darkstone"),
		Block::ReturnBlock("Mossy Darkstone"),
		Block::ReturnBlock("Stone Brick"),
		Block::ReturnBlock("Mossy Stone Brick"),
		Block::ReturnBlock("Cracked Stone Brick"),
		Block::ReturnBlock("Chest")
	});





	//WorldStructure Ruins(std::string("Ruins"), &Stone, &Leaves);

	//Calls for checking for the scroll value
	activeCamera = &camera;
	glfwSetScrollCallback(window, scroll_callback);



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//Defines IO
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true); 
	ImGui_ImplOpenGL3_Init("#version 330");

	float DevMenuFontSize = 18.0f;

	ImGuiStyle& DevMenuStyle = ImGui::GetStyle();
	DevMenuStyle.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	//Font Definitions, using IO

	ImFont* SpaceMono_Regular = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Regular.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
	ImFont* SpaceMono_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());

	DevMenuStyle.WindowBorderSize = 0.0f;




	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	//Block::TotalBlockCount = 0;

	//Main program while loop
	while (!glfwWindowShouldClose(window))
	{
		deltatime.CalculateValues();
		float currentTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Tells OpenGL which shader program to use
		shaderProgram.Activate();


		sceneFBO.Bind();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Sunlight properties
		//Super basic day/night cycle, applies the current time to the X rotation

		glm::vec3 sunDir = glm::normalize(glm::vec3(currentTime, -1.0f, -0.2f));

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "sunDirection"), sunDir.x, sunDir.y, sunDir.z);


		glUniform3f(glGetUniformLocation(shaderProgram.ID, "sunColor"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "sunStrength"), 0.8f);



		//ONLY FOR basic-pixelation SHADERS!
	
		//glUniform1f(glGetUniformLocation(shaderProgram.ID, "pixelSize"), 8.0f);  //divides the pixels by this value, so 16 / 8 would be a 2x2 texture
		//glUniform2f(glGetUniformLocation(shaderProgram.ID, "textureSize"), 16.0f, 16.0f); // texture size


		camera.Inputs(window);
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		camera.Matrix(FOV, 0.1f, 100.0f, shaderProgram, "camMatrix", width, height);


		VAO1.Bind();
		//Draws the shape using the GL Primative, Triangle
	
		float t = glfwGetTime();
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "time"), t);


		//Builds a manual set of blocks
		for (int i = 0; i < blockDataBaseLength; i++)
		{
			Block::SpawnBlock(allBlocks[i].DisplayName, glm::vec3(((i * 1.0f) - (blockDataBaseLength / 2)), 0.0f, -20.0f), shaderProgram);
		}

		//Spawns the 2 trees
		Tree::SpawnTree(glm::vec3(0.0f, 0.0f, 5.0f), OakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(0.0f, 0.0f, 12.0f), LargeOakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(0.0f, 0.0f, 19.0f), PineTree, shaderProgram);

		//Spawns the 2 foliage
		Tree::SpawnTree(glm::vec3(5.0f, 0.0f, 5.0f), FallenLog, shaderProgram);
		Tree::SpawnTree(glm::vec3(5.0f, 0.0f, 10.0f), Rock, shaderProgram);

		//Spawns the Ruin Structure
		WorldStructure::SpawnStructure(glm::vec3(15.0f, -1.0f, 10.0f), Ruins, shaderProgram);

		//Spawns the stone floor
		Block::SpawnAreaOfBlocks("Stone", glm::vec3(15.0f, -1.0f, 10.0f), glm::vec2(-3.0f, 4.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-3.0f, 4.0f), shaderProgram);

		// ! WARNING !, this is a temporary solution, and murders the FPS.
		Block::SpawnAreaOfBlocks("Grass Top", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(-25.0f, 25.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-25.0f, 25.0f), shaderProgram);


		Block::SpawnBlock("Light Block", glm::vec3(0.0f, 1.0f, -3.0f), shaderProgram);


		//for (int i = 0; i < Tree::allTreePositions.size(); i++)
		//{
			//Tree::SpawnTree(Tree::allTreePositions[i], OakTree, shaderProgram);
		//}




		//Spawns a block on the camera's current position
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			allPlacedBlocksPos.push_back(camera.Position);
		}

		//Re-renders all the placed blocks, if they are any
		if (allPlacedBlocksPos.empty() == false)
		{
			for (int i = 0; i < allPlacedBlocksPos.size(); i++)
			{
				Block::SpawnBlock("Stone", allPlacedBlocksPos[i], shaderProgram);
			}
		}


		sceneFBO.Unbind();

		// bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// disable depth so UI doesn't fight it
		glDisable(GL_DEPTH_TEST);

		// use screen shader
		screenShader.Activate();
		


		if (screenShader.VertexShaderName == "pixelation.vert")
		{
			screenShader.setFloat("pixelSize", 4.0f);  //The resolution is divided by this value, so lower = higher res
			screenShader.setVec2("resolution", (float)width, (float)height);
		}



		glBindTexture(GL_TEXTURE_2D, sceneFBO.textureID);
		quadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(800, 800));

		//All the display data for the developer menu
		ImGui::Begin("DEVELOPER MENU", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		//Using the default font right now
		//ImGui::PushFont(SpaceMono_Regular);
		ImGui::Text("FPS: %.2f", deltatime.FPS);
		ImGui::Text("Delta Time: %.10fs", deltatime.DTPS);
		ImGui::Text("Runtime: %.1f seconds", currentTime);
		//ImGui::Text("Block Count %i", Block::TotalBlockCount);
		ImGui::Text("Camera Speed %.1f", camera.speed);
		ImGui::Text("Camera Position %.1f, %.1f, %.1f", camera.Position.x, camera.Position.y, camera.Position.z);
		ImGui::Text("[L-Alt] Camera Free: %s", camera.ImitatePlayer ? "FALSE" : "TRUE");
		ImGui::Text("Engine Rendering Mode: %s", camera.RenderModeDisplayName);
		ImGui::Text("Applying Shader: %s and %s", screenShader.VertexShaderName, screenShader.FragmentShaderName);

		//When using custom fonts, make sure to UNCOMMENT this
		//ImGui::PopFont();
		ImGui::End();



		// health bar window
		ImGui::SetNextWindowPos(ImVec2(width - (width * 0.1), 20), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(800, 800));

		ImGui::Begin("Scene Collection", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::Text("Voxel Landscape");
		ImGui::End();

		//IM GUI SHOULD ALWAYS RENDER AFTER THE EVERYTHING ELSE
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

		//While window is not closed, allow for input and such.
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	//Deletes all the block textures
	for (int i = 0; i < blockTextureBaseLength; i++)
	{
		allBlockTextures[i].Delete();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}