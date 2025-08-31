#pragma once
#include "Texture.h"
#include"shaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>


class Block
{
public:
	Block(std::string ID, glm::vec3 position);

	//Defines the block types
	enum BlockType
	{
		Natural, 
		Ore,
		Liquid,
		Crafted
	};

	//Defines the Block Data
	struct BlockData
	{
		//Used for identification
		std::string DisplayName;
		BlockType type;

		//The raw texture path
		std::string texturePath;
		float blockDurability;
		bool emmitsLight = false;

		//Used to "link" the texture object with the block
		int blockTextureIndex;
	};

	//static int TotalBlockCount;

	//Creates the array of Blocks, that can later be used as a "Data Table"
	static std::vector<BlockData> blockDatabase;

	//Creates the array of Texture objects, which should be in the same exact order as the Blocks Array
	static std::vector<Texture> allBlockTextures;

	std::string blockID;
	glm::vec3 Position;

	//This function links the blockTextureIndex to the Block Texture array
	static void InitTextures();

	//Used for returning a block given the input Display Name
	static Block::BlockData ReturnBlock(std::string BlockDataID);

	//This function spawns the block
	static void SpawnBlock(std::string BlockDataID, glm::vec3 position, Shader shaderProgram);


	//Used for drawing a square of blocks
	static void SpawnAreaOfBlocks(std::string BlockDataID, glm::vec3 position, glm::vec2 sizeX, glm::vec2 sizeY, glm::vec2 sizeZ, Shader shaderProgram);
	
	
	static std::vector<glm::vec3> allBlockPositions;
	static bool doesBlockExsistAtLocation(glm::vec3);

};