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
#include"VoxlEngine.h"
#include"Tree.h"
#include"Block.h"
#include"WorldStructure.h"
#include"FBO.h"
#include"MenuBar.h"
#include"UserInterface.h"

#define FMT_HEADER_ONLY
#include<core.h>


//Its cleaner if I just put this inside Camera.h
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

	//Defines the voxlEngine object to be used in the rest of the script
	VoxlEngine voxlEngine;

	MenuBar menuBar;

	UI_Tools ui_Tools;

	//Rendering resolution can be fixed like this
	int width = 2560;
	int height = 1440;

	//Or it can take up the entire screen (still windowed) like this
	//int width = voxlEngine.getDisplayResolution().x;
	//int height = voxlEngine.getDisplayResolution().y;
	
	
	//Defines the window object
	GLFWwindow* window = glfwCreateWindow(width, height, "Voxl Engine", NULL, NULL);


	//Creates the top Menu Bar
	menuBar.CreateMenuBar(window, glfwGetWin32Window(window));

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
	//glClearColor(0.0, 0.0, 0.0, 1.0f); //== Black

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
	unsigned char* iconPixels = stbi_load("C:/dev/Voxl-Engine/Images/Branding/TerracubeGradient_Small.png", &iconWidth, &iconHeight, &iconChannels, 4);


	//Changes the window icon
	GLFWimage images[1];
	images[0].width = iconWidth;
	images[0].height = iconHeight;
	images[0].pixels = iconPixels;

	glfwSetWindowIcon(window, 1, images);


	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

	//Causing issues because I moved it into the shaders folder
	// or "../../Shaders/" depending where you are
	Shader screenShader("Shaders/pixelation.vert", "Shaders/pixelation.frag");


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

	float DevMenuFontSize = 16.0f;

	ImGuiStyle& DevMenuStyle = ImGui::GetStyle();
	DevMenuStyle.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	DevMenuStyle.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	//Font Definitions, using IO

	ImFont* SpaceMono_Regular = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Regular.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
	ImFont* SpaceMono_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/SpaceMono-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());

	ImFont* Doto = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Doto-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
	ImFont* Doto_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Doto-Black.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());

	ImFont* Pixeloid = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Pixeloid.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());
	ImFont* Pixeloid_Bold = io.Fonts->AddFontFromFileTTF("C:/dev/Voxl-Engine/Fonts/Pixeloid-Bold.ttf", DevMenuFontSize, NULL, io.Fonts->GetGlyphRangesDefault());

	DevMenuStyle.WindowBorderSize = 0.0f;




	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	//Block::TotalBlockCount = 0;

	//Main program while loop
	while (!glfwWindowShouldClose(window))
	{
		voxlEngine.CalculateFPS();

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
	
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "time"), currentTime);


		//Builds a manual set of blocks
		//for (int i = 0; i < blockDataBaseLength; i++)
		//{
			//Block::SpawnBlock(allBlocks[i].DisplayName, glm::vec3(((i * 1.0f) - (blockDataBaseLength / 2)), 0.0f, -20.0f), shaderProgram);
		//}

		//Spawns trees
		Tree::SpawnTree(glm::vec3(0.0f, 0.0f, 5.0f), OakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(10.0f, 0.0f, 3.0f), OakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(12.0f, 0.0f, -5.0f), OakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(-3.0f, 0.0f, 12.0f), LargeOakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(3.0f, 0.0f, -5.0f), LargeOakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(15.0f, 0.0f, -10.0f), LargeOakTree, shaderProgram);
		Tree::SpawnTree(glm::vec3(0.0f, 0.0f, 19.0f), PineTree, shaderProgram);

		//Spawns the 2 foliage
		Tree::SpawnTree(glm::vec3(5.0f, 0.0f, 5.0f), FallenLog, shaderProgram);
		Tree::SpawnTree(glm::vec3(5.0f, 0.0f, 10.0f), Rock, shaderProgram);
		Tree::SpawnTree(glm::vec3(15.0f, 0.0f, 3.0f), Rock, shaderProgram);

		//Spawns the Ruin Structure
		WorldStructure::SpawnStructure(glm::vec3(15.0f, -1.0f, 10.0f), Ruins, shaderProgram);

		//Spawns the stone floor
		Block::SpawnAreaOfBlocks("Stone", glm::vec3(15.0f, -1.0f, 10.0f), glm::vec2(-3.0f, 4.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-3.0f, 4.0f), shaderProgram);

		// ! WARNING !, this is a temporary solution, and murders the FPS.
		Block::SpawnAreaOfBlocks("Grass Top", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(-25.0f, 25.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-25.0f, 25.0f), shaderProgram);

		Block::SpawnBlock("Light Block", glm::vec3(0.0f, 1.0f, -3.0f), shaderProgram);



		//Spawns a block on the camera's current position

		glm::vec3 blockPlacementPosition = camera.Position + voxlEngine.multiplyVectorWithFloat(camera.Orientation, 2.0f);

		//glm::vec3 blockPlacementPosition = voxlEngine.Raycast(camera.Position, camera.Orientation, 128).hitPosition; 


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			allPlacedBlocksPos.push_back(blockPlacementPosition);
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
		


		if (screenShader.VertexShaderName == "Shaders/pixelation.vert")
		{
			screenShader.setFloat("pixelSize", 1.0f);  //The resolution is divided by this value, so lower = higher res
			screenShader.setVec2("resolution", (float)width, (float)height);
		}
		glBindTexture(GL_TEXTURE_2D, sceneFBO.textureID);
		quadVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//User Interface

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Developer Canvas
		Canvas DeveloperMenu(ImVec2(20, 20), ImVec2(800, 800), "DEVELOPER MENU");

		//Perfomance Metrics
		TextBlock txt_FPS(DeveloperMenu, fmt::format("FPS: {:.2f}", voxlEngine.FPS));
		TextBlock txt_DeltaTime(DeveloperMenu, fmt::format("Delta Time: {:.10f}", voxlEngine.DTPS));
		TextBlock txt_RunTime(DeveloperMenu, fmt::format("Runtime: {:.1f} seconds", currentTime));


		//Camera
		TextBlock txt_buffer01(DeveloperMenu, " ");
		TextBlock txt_CameraSpeed(DeveloperMenu, fmt::format("Camera Speed {:.1f}", camera.speed));
		TextBlock txt_CameraRotation(DeveloperMenu, fmt::format("Camera Rotation {:.1f}, {:.1f}, {:.1f}", camera.Orientation.x, camera.Orientation.y, camera.Orientation.z));
		TextBlock txt_CameraPosition(DeveloperMenu, fmt::format("Camera Position {:.1f}, {:.1f}, {:.1f}", camera.Position.x, camera.Position.y, camera.Position.z));

		//Misc UI
		TextBlock txt_buffer02(DeveloperMenu, " ");
		TextBlock txt_cameraMode(DeveloperMenu, fmt::format("[L-Alt] Camera Free: {:s}", camera.ImitatePlayer ? "FALSE" : "TRUE"));
		TextBlock txt_resolution(DeveloperMenu, fmt::format("Display Resolution: {:1}/{:1}", voxlEngine.getDisplayResolution().x, voxlEngine.getDisplayResolution().y));


		//Renders the DeveloperMenu
		Canvas::Render(DeveloperMenu);


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