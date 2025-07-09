#pragma once
#include <vector>
#include "Texture.h"
#include "Block.h"
#include <iostream>
#include "shaderClass.h"

class WorldStructure
{
public:
	struct StructurePart
	{
		Block::BlockData block;
		std::vector<glm::vec3> relativePos;
	};

	std::vector<StructurePart> StructureParts;

	static void SpawnStructure(glm::vec3 position, WorldStructure structureObject, Shader shaderProgram);

	WorldStructure(std::string StructureID, std::vector<Block::BlockData> blocks);
};