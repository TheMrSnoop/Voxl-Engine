#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>

class VoxlEngine
{
public:
	VoxlEngine();
	glm::vec2 getDisplayResolution();
	glm::vec3 getForwardVector();
	
	glm::vec3 multiplyVectorWithFloat(glm::vec3, float);
	
	//For calculating FPS and DeltaTime
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	float DTPS = 0.0f;
	float FPS = 0.0f;
	int roundedFPS = 0;
	void CalculateFPS();


	struct RaycastHit
	{
		glm::vec3 blockHitPosition;
		glm::vec3 hitPosition;
		glm::vec3 hitNormal;
		bool hit;
	};

	RaycastHit Raycast(glm::vec3 origin, glm::vec3 dir, int maxSteps);
private:
	//Also only for calculating FPS and DeltaTime
	float lastTime = 0.0f;
	float timeAccumulator = 0.0f;
	float delay = 1.0f;
};