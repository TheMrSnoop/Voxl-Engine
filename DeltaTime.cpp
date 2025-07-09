#include <iostream>
#include "DeltaTime.h"

DeltaTime::DeltaTime(float delay)
{
	this->delay = delay;
	lastTime = glfwGetTime();
	timeAccumulator = 0.0f;
}

void DeltaTime::CalculateValues()
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
