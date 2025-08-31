#include "VoxlEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include "Camera.h"
#include "Block.h"

float VoxlEngine::FPS = 0.0f;
float VoxlEngine::DTPS = 0.0f;

VoxlEngine::VoxlEngine()
{
	//Initalizes for FPS calculations
	lastTime = glfwGetTime();
	timeAccumulator = 0.0f;
}

glm::vec2 VoxlEngine::getDisplayResolution()
{
	//Gets the primaryMonitor
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	//Returns the primaryMonitor's display res
	glm::vec2 displayResolution = glm::vec2(mode->width, mode->height);
	return displayResolution;
}


void VoxlEngine::CalculateFPS()
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	timeAccumulator += deltaTime;

	if (timeAccumulator >= delay)
	{
		FPS = 1.0f / deltaTime;
		roundedFPS = round(FPS);
		DTPS = deltaTime;


		timeAccumulator = 0.0f;
	}
}

int VoxlEngine::getRandomInt(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

std::string VoxlEngine::getRandomVectorMember_STR(std::vector<std::string> inputVector)
{
	int MaxIndex = inputVector.size() - 1;

	return (inputVector[getRandomInt(0, MaxIndex)]);
}

glm::vec3 VoxlEngine::getForwardVector()
{
	float yaw = activeCamera->Orientation.x;
	float pitch = activeCamera->Orientation.y;

	std::cout << "Camera Rotation: " << activeCamera->Orientation.x << ", " << activeCamera->Orientation.y << ", " << activeCamera->Orientation.z << std::endl;
	glm::vec3 cameraFront;
	cameraFront.x = cos(pitch) * sin(yaw);
	cameraFront.y = -sin(pitch);
	cameraFront.z = cos(pitch) * cos(yaw);

	return glm::normalize(cameraFront);
}


glm::vec3 VoxlEngine::multiplyVectorWithFloat(glm::vec3 inVector, float multiplier)
{
	glm::vec3 productVector;

	productVector.x = inVector.x * multiplier;
	productVector.y = inVector.y * multiplier;
	productVector.z = inVector.z * multiplier;

	return productVector;
}

bool VoxlEngine::isVectorEqual(glm::vec3 vec1, glm::vec3 vec2)
{
	if (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z) {
		return true;
	}
	else {
		return false;
	}
}


VoxlEngine::RaycastHit VoxlEngine::Raycast(glm::vec3 origin, glm::vec3 dir, int maxSteps)
{
	RaycastHit hitResult;

	glm::vec3 voxelPosition = glm::floor(origin);
	glm::vec3 step = glm::sign(dir);

	glm::vec3 tDelta = glm::abs(1.0f / dir); // how far to cross a voxel
	glm::vec3 tMax;

	glm::vec3 voxelBorder = glm::vec3(voxelPosition) + glm::step(glm::vec3(0), dir);
	tMax = (voxelBorder - origin) / dir;


	for (int i = 0; i < maxSteps; ++i) {
		// check if current voxel is solid
		if (Block::doesBlockExsistAtLocation(voxelPosition)) {
			hitResult.blockHitPosition = voxelPosition;
			hitResult.hit = true;

			// calc exact hit position
			glm::vec3 travel = dir * glm::min(tMax.x, glm::min(tMax.y, tMax.z));
			hitResult.hitPosition = origin + travel;

			// determine hit face normal (direction we stepped back from)
			if (tMax.x < tMax.y && tMax.x < tMax.z)
				hitResult.hitNormal = glm::ivec3(-step.x, 0, 0);
			else if (tMax.y < tMax.z)
				hitResult.hitNormal = glm::ivec3(0, -step.y, 0);
			else
				hitResult.hitNormal = glm::ivec3(0, 0, -step.z);

			return hitResult;
		}

		// advance ray to next voxel
		if (tMax.x < tMax.y && tMax.x < tMax.z) {
			tMax.x += tDelta.x;
			voxelPosition.x += step.x;
		}
		else if (tMax.y < tMax.z) {
			tMax.y += tDelta.y;
			voxelPosition.y += step.y;
		}
		else {
			tMax.z += tDelta.z;
			voxelPosition.z += step.z;
		}
	}

	// no hit, return end of trace
	hitResult.hit = false;
	hitResult.hitPosition = origin + dir * glm::min(tMax.x, glm::min(tMax.y, tMax.z));
	return hitResult;
}