#include"Camera.h"
#include"Terrain.h"
#include<iostream>



Camera* activeCamera = nullptr;

//Ties the camera speed to the scroll value
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (activeCamera)
	{
		activeCamera->speed += yoffset * 0.5f;
		activeCamera->speed = glm::clamp(activeCamera->speed, 0.0001f, 50.0f);
		std::cout << "scroll speed: " << activeCamera->speed << std::endl;
	}
}


Camera::Camera(int width, int height, glm::vec3 position)
{
	//Initializes varriables

	Camera::width = width;
	Camera::height = height;
	Position = position;
}


void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, int width, int height)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

float lastTime = glfwGetTime();

void Camera::ConfigureRenderMode(Camera::RenderMode InputRenderMode)
{
	CurrentRenderMode = InputRenderMode;

	switch (CurrentRenderMode)
	{
	case Camera::Lit:
		RenderModeDisplayName = "Lit";
		break;
	case Camera::Unlit:
		RenderModeDisplayName = "Unlit";
		break;
	case Camera::Wireframe:
		RenderModeDisplayName = "Wireframe";
		break;
	default:
		break;
	}

	std::cout << RenderModeDisplayName << std::endl;
}

void Camera::Inputs(GLFWwindow* window)
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	fps = 1 / deltaTime;


	//Engine Input and Controls

	bool RequireMouseInputForMovement = false;

	if (!RequireMouseInputForMovement || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		//WASD CONTROLS
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += speed * deltaTime * Orientation;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += speed * deltaTime * -glm::normalize(glm::cross(Orientation, Up));
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += speed * deltaTime * -Orientation;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += speed * deltaTime * glm::normalize(glm::cross(Orientation, Up));
		}



		//OTHER CONTROLS
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			Position += speed * deltaTime * Up;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			Position += speed * deltaTime * -Up;
		}
	}
	


	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		ConfigureRenderMode(Camera::RenderMode::Lit);
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		ConfigureRenderMode(Camera::RenderMode::Unlit);
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		ConfigureRenderMode(Camera::RenderMode::Wireframe);
	}



	//MOUSE CONTROLS
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{


		//Hides the cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		//Prevents the camera from jumping around on the first click
		if (FirstClick)
		{
			//Resets the cursor to the center of the screen. /2 because that is 50%.
			glfwSetCursorPos(window, (width / 2), (height / 2));
			FirstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);


		float rotX = sensitivity * (float)(mouseY - (height / 2)) / (float)height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / (float)width;
		
		static bool altWasPressed = false;

		bool altIsPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;

		if (altIsPressed && !altWasPressed) {
			ImitatePlayer = !ImitatePlayer;
		}

		altWasPressed = altIsPressed;


		if (ImitatePlayer)
		{
			//This locks the camera position to the top of the the chunks, to immiate a basic player camera
			unsigned int PlayerHeight = 2;
			Position = glm::vec3(Position.x, Chunk::ChunkHeight + PlayerHeight, Position.z);
			speed = 4.5f;
		}


		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) or glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))
		{
			//Makes sure that you cannot do barrel rolls and flip upside down before applying the new Orientation

			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		//Resets the cursor to the center of the screen. /2 because that is 50%.
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		FirstClick = true;
	}
}