#include"WorldStructure.h"
#include <iostream>
#include"Block.h"




void WorldStructure::SpawnStructure(glm::vec3 position, WorldStructure structureObject, Shader shaderProgram)
{
	for (int p = 0; p < structureObject.StructureParts.size(); p++)
	{
		for (auto& offset : structureObject.StructureParts[p].relativePos)
		{
			Block::SpawnBlock(structureObject.StructureParts[p].block.DisplayName, position + offset, shaderProgram);
		}
	}
}


WorldStructure::WorldStructure(std::string StructureID, std::vector<Block::BlockData> blocks)
{
	if (StructureID == "Ruins")
	{
		StructurePart cleanFloors;
		cleanFloors.block = blocks[0];
		cleanFloors.relativePos =
		{
			{0.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f},

			{-1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, -1.0f},
		};
		//Adds the structure
		this->StructureParts.push_back(cleanFloors);


		StructurePart mossyFloors;
		mossyFloors.block = blocks[1];
		mossyFloors.relativePos =
		{
			{0.0f, 0.0f, -1.0f},
			{-1.0f, 0.0f, -1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
		};
		//Adds the structure
		this->StructureParts.push_back(mossyFloors);


		StructurePart NormalBrickWalls;
		NormalBrickWalls.block = blocks[2];
		NormalBrickWalls.relativePos =
		{

			//Entrance Left
			{2.0f, 1.0f, 1.0f},

			//Corners
			{2.0f, 1.0f, 2.0f},


			{-2.0f, 1.0f, 2.0f},
			{-2.0f, 2.0f, 2.0f},

			{-2.0f, 1.0f, -2.0f},


			{2.0f, 1.0f, -2.0f},
			{2.0f, 2.0f, -2.0f},


			//Left Wall
			{-1.0f, 1.0f, 2.0f},
			{-1.0f, 2.0f, 2.0f},
			{-2.0f, 1.0f, 2.0f},
			{-2.0f, 2.0f, 2.0f},

			//Right Wall
			{1.0f, 1.0f, -2.0f},


			{-1.0f, 2.0f, -2.0f},

			{-2.0f, 2.0f, -2.0f},


			//Back Wall

			{-2.0f, 1.0f, 0.0f},

			{-2.0f, 1.0f, -1.0f},
			{-2.0f, 2.0f, -1.0f},
			{-2.0f, 3.0f, -1.0f},
		};
		//Adds the structure
		this->StructureParts.push_back(NormalBrickWalls);


		StructurePart MossyBrickWalls;
		MossyBrickWalls.block = blocks[3];
		MossyBrickWalls.relativePos =
		{
			//Corners
			{2.0f, 2.0f, 2.0f},
			{2.0f, 3.0f, 2.0f},

			//Left Wall
			{1.0f, 1.0f, 2.0f},
			{0.0f, 1.0f, 2.0f},

			//Right Wall
			{-2.0f, 1.0f, -2.0f},
			{0.0f, 1.0f, -2.0f},
			{1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, -2.0f},




		};
		this->StructureParts.push_back(MossyBrickWalls);


		StructurePart CrackedBrickWalls;
		CrackedBrickWalls.block = blocks[4];
		CrackedBrickWalls.relativePos =
		{
			//Walls
			{-2.0f, 2.0f, 0.0f},
			{-2.0f, 1.0f, 1.0f},
			{-2.0f, 2.0f, -2.0f},
			{-2.0f, 3.0f, -2.0f},
		};
		this->StructureParts.push_back(CrackedBrickWalls);


		StructurePart Chest;
		Chest.block = blocks[5];
		Chest.relativePos =
		{
			//Walls
			{0.0f, 1.0f, 0.0f},
		};
		this->StructureParts.push_back(Chest);
	}
}