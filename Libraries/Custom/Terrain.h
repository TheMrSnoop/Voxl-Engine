#pragma once
#include "Block.h"

class Chunk
{
public:
	Chunk(glm::vec3 inputCordinates, Shader shaderProgram);

	static void SpawnChunks(glm::uint Iterations, Shader shaderProgram);
	 
	glm::vec3 cordinates;
	std::vector<Block> TopLayer;
};