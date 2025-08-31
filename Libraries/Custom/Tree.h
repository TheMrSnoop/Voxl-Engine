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

	static std::vector<Tree> AllTrees;

	struct TreePart {
		Block::BlockData treeBlock;
		std::vector<glm::vec3> relativePos;
	};

	std::vector<TreePart> parts; 

	glm::vec3 TreePosition;

	static void RenderAllTrees(Shader shaderProgram);

	Tree(const std::string TreeDisplayName, const Block::BlockData& trunkBlock, const Block::BlockData& leafBlock, glm::vec3 InputTreePosition);
};
