#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp> 
#include "Texture.h"
#include "Block.h"
#include"shaderClass.h"

class Tree
{
public:
	struct TreePart {
		Block::BlockData treeBlock;
		std::vector<glm::vec3> relativePos;
	};

	std::vector<TreePart> parts; 

	
	//static std::vector<glm::vec3> allTreePositions;

	static void fillOutRandomTreePositions(glm::vec2 sizeX, glm::vec2 sizeZ, float height, int TreeCount);

	static void SpawnTree(glm::vec3 TreePosition, Tree treeObject, Shader shaderProgram);


	Tree(const std::string& TreeDisplayName, const Block::BlockData& trunkBlock, const Block::BlockData& leafBlock);
};
