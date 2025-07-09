#pragma once

#include <GLFW/glfw3.h>

class DeltaTime
{
public:
	float currentTime = 0.0f;


	float deltaTime = 0.0f;
	float DTPS = 0.0f;
	float FPS = 0.0f;

	int roundedFPS = 0;

	DeltaTime(float delay);

	void CalculateValues();

private:
	float lastTime = 0.0f;
	float timeAccumulator = 0.0f;
	float delay = 1.0f;
};
