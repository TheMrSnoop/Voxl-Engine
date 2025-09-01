#pragma once

#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class PerlinNoise
{
public:
	static const unsigned int gridSize = 16;

	static float Perlin(float x, float y);

	static int computeHeightFromPerlinColumn(int baseMaxY, float chunkWorldX, float chunkWorldZ, 
		float freq = 0.02f, float amplitude = 1.0f,
		int quantizeLevels = 2, uint32_t seed = 0u, 
		int minClamp = 0, int maxClamp = INT_MAX);
};