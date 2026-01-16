#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
public:
	enum RenderMode
	{
		Lit,
		Unlit,
		Wireframe
	};

	//Matrix
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view;
	glm::mat4 projection;

	bool FirstClick = true;

	bool ImitatePlayer = false;

	int width;
	int height;

	float speed = 10.0f;
	float sensitivity = 500.0f;

	float deltaTime = 0.0f;
	float fps = 0.0f;

	Camera::RenderMode CurrentRenderMode = Lit;

	std::string RenderModeDisplayName = "Lit";

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, int width, int height);

	void ConfigureRenderMode(Camera::RenderMode InputRenderMode);

	void Inputs(GLFWwindow* window);
};


extern Camera* activeCamera;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif
