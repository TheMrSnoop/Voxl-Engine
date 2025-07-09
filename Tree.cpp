#include"Tree.h"
#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include <random>


void Tree::SpawnTree(glm::vec3 TreePosition, Tree treeObject, Shader shaderProgram)
{
	//Cycles through both the log and leaf parts to build the tree.
	// (Since the trunk is defined first, it builds the trunk, then the leaves)



	for (int p = 0; p < treeObject.parts.size(); p++)
	{
		for (auto& offset : treeObject.parts[p].relativePos)
		{
			Block::SpawnBlock(treeObject.parts[p].treeBlock.DisplayName, TreePosition + offset, shaderProgram);
		}
	}
}


int getRandomInt(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

void Tree::fillOutRandomTreePositions(glm::vec2 sizeX, glm::vec2 sizeZ, float height, int TreeCount)
{
	for (int i = 0; i < TreeCount; i++)
	{

		float generatedX = (getRandomInt(round(sizeX.x), round(sizeX.y)) * 1.0f);
		float generatedZ = (getRandomInt(round(sizeZ.x), round(sizeZ.y)) * 1.0f);

		glm::vec3 generatedVec3 = glm::vec3(generatedX, height, generatedZ);

		//Tree::allTreePositions.push_back(generatedVec3);

	}
}





Tree::Tree(const std::string& TreeDisplayName, const Block::BlockData& trunkBlock, const Block::BlockData& leafBlock)
{
	if (TreeDisplayName == "Oak")
	{
		//Trunk
		TreePart trunkPart;
		trunkPart.treeBlock = trunkBlock;
		trunkPart.relativePos =
		{
			{0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 2.0f, 0.0f},
			{0.0f, 3.0f, 0.0f}
		};
		this->parts.push_back(trunkPart);


		//Leaf
		TreePart leafPart;
		leafPart.treeBlock = leafBlock;
		leafPart.relativePos =
		{
			//Center blocks
			{0.0f, 4.0f, 0.0f},
			{0.0f, 5.0f, 0.0f},
			{0.0f, 6.0f, 0.0f},
			{0.0f, 7.0f, 0.0f},
			//The outmost edges
			{1.0f, 4.0f, 0.0f},
			{1.0f, 5.0f, 0.0f},
			{2.0f, 4.0f, 0.0f},
			{2.0f, 5.0f, 0.0f},

			{-1.0f, 4.0f, 0.0f},
			{-1.0f, 5.0f, 0.0f},
			{-2.0f, 4.0f, 0.0f},
			{-2.0f, 5.0f, 0.0f},

			{0.0f, 4.0f, 1.0f},
			{0.0f, 5.0f, 1.0f},
			{0.0f, 4.0f, 2.0f},
			{0.0f, 5.0f, 2.0f},

			{0.0f, 4.0f, -1.0f},
			{0.0f, 5.0f, -1.0f},
			{0.0f, 4.0f, -2.0f},
			{0.0f, 5.0f, -2.0f},
			//Filling it in
			{1.0f, 4.0f, 1.0f},
			{1.0f, 5.0f, 1.0f},

			{2.0f, 4.0f, 1.0f},
			{2.0f, 5.0f, 1.0f},

			{1.0f, 4.0f, 2.0f},
			{1.0f, 5.0f, 2.0f},
			//{2.0f, 4.0f, 2.0f},
			//{2.0f, 5.0f, 2.0f},
			{-1.0f, 4.0f, 1.0f},
			{-1.0f, 5.0f, 1.0f},

			{-2.0f, 4.0f, 1.0f},
			{-2.0f, 5.0f, 1.0f},

			{-1.0f, 4.0f, 2.0f},
			{-1.0f, 5.0f, 2.0f},

			{-2.0f, 4.0f, 2.0f},
			//{-2.0f, 5.0f, 2.0f},
			{1.0f, 4.0f, -1.0f},
			{1.0f, 5.0f, -1.0f},

			{2.0f, 4.0f, -1.0f},
			{2.0f, 5.0f, -1.0f},

			{1.0f, 4.0f, -2.0f},
			{1.0f, 5.0f, -2.0f},
			//{2.0f, 4.0f, -2.0f},
			//{2.0f, 5.0f, -2.0f},
			{-1.0f, 4.0f, -1.0f},
			{-1.0f, 5.0f, -1.0f},

			{-2.0f, 4.0f, -1.0f},
			{-2.0f, 5.0f, -1.0f},

			{-1.0f, 4.0f, -2.0f},
			{-1.0f, 5.0f, -2.0f},

			{-2.0f, 4.0f, -2.0f},
			{-2.0f, 5.0f, -2.0f},


			//The very top
			{ 1.0f, 6.0f, 0.0f },
			{ -1.0f, 6.0f, 0.0f },
			{ 0.0f, 6.0f, -1.0f },
			{ 0.0f, 6.0f, 1.0f },
		};


		this->parts.push_back(leafPart);

	}
	else if (TreeDisplayName == "Oak_Large")
	{
		//Trunk
		TreePart trunkPart;
		trunkPart.treeBlock = trunkBlock;
		trunkPart.relativePos =
		{
			//Trunk
			{0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 2.0f, 0.0f},
			{0.0f, 3.0f, 0.0f},
			{0.0f, 4.0f, 0.0f},
			{0.0f, 5.0f, 0.0f},
			{0.0f, 6.0f, 0.0f},
			{0.0f, 7.0f, 0.0f},

			//Branch
			{1.0f, 4.0f, 0.0f},
			{2.0f, 4.0f, 0.0f},
			{2.0f, 5.0f, 0.0f},
		};
		this->parts.push_back(trunkPart);

		//Leaf
		TreePart leafPart;
		leafPart.treeBlock = leafBlock;
		leafPart.relativePos =
		{
			//Center blocks
			{0.0f, 8.0f, 0.0f},
			{0.0f, 9.0f, 0.0f},
			{0.0f, 10.0f, 0.0f},
			//The outmost edges
			{1.0f, 8.0f, 0.0f},
			{1.0f, 9.0f, 0.0f},
			{2.0f, 8.0f, 0.0f},
			{2.0f, 9.0f, 0.0f},

			{-1.0f, 8.0f, 0.0f},
			{-1.0f, 9.0f, 0.0f},
			{-2.0f, 8.0f, 0.0f},
			{-2.0f, 9.0f, 0.0f},

			{0.0f, 8.0f, 1.0f},
			{0.0f, 9.0f, 1.0f},
			{0.0f, 8.0f, 2.0f},
			{0.0f, 9.0f, 2.0f},

			{0.0f, 8.0f, -1.0f},
			{0.0f, 9.0f, -1.0f},
			{0.0f, 8.0f, -2.0f},
			{0.0f, 9.0f, -2.0f},
			//Filling it in
			{1.0f, 8.0f, 1.0f},
			{1.0f, 9.0f, 1.0f},

			{2.0f, 8.0f, 1.0f},
			{2.0f, 9.0f, 1.0f},

			{1.0f, 8.0f, 2.0f},
			{1.0f, 9.0f, 2.0f},
			//{2.0f, 8.0f, 2.0f},
			//{2.0f, 9.0f, 2.0f},
			{-1.0f, 8.0f, 1.0f},
			{-1.0f, 9.0f, 1.0f},

			{-2.0f, 8.0f, 1.0f},
			{-2.0f, 9.0f, 1.0f},

			{-1.0f, 8.0f, 2.0f},
			{-1.0f, 9.0f, 2.0f},

			{-2.0f, 8.0f, 2.0f},
			//{-2.0f, 9.0f, 2.0f},
			{1.0f, 8.0f, -1.0f},
			{1.0f, 9.0f, -1.0f},

			{2.0f, 8.0f, -1.0f},
			{2.0f, 9.0f, -1.0f},

			{1.0f, 8.0f, -2.0f},
			{1.0f, 9.0f, -2.0f},
			//{2.0f, 8.0f, -2.0f},
			//{2.0f, 9.0f, -2.0f},
			{-1.0f, 8.0f, -1.0f},
			{-1.0f, 9.0f, -1.0f},

			{-2.0f, 8.0f, -1.0f},
			{-2.0f, 9.0f, -1.0f},

			{-1.0f, 8.0f, -2.0f},
			{-1.0f, 9.0f, -2.0f},

			{-2.0f, 8.0f, -2.0f},
			{-2.0f, 9.0f, -2.0f},



			//The very bottom
			{ 1.0f, 7.0f, 0.0f },
			{ -1.0f, 7.0f, 0.0f },
			{ 0.0f, 7.0f, -1.0f },
			{ 0.0f, 7.0f, 1.0f },

			//The very top
			{ 1.0f, 10.0f, 0.0f },
			{ -1.0f, 10.0f, 0.0f },
			{ 0.0f, 10.0f, -1.0f },
			{ 0.0f, 10.0f, 1.0f },

			{ 1.0f, 10.0f, 1.0f },
			{ -1.0f, 10.0f, 1.0f },
			{ -1.0f, 10.0f, -1.0f },
			{ 1.0f, 10.0f, -1.0f },


			//Branch Leaves
			{ 2.0f, 6.0f, 0.0f },
			{ 1.0f, 5.0f, 0.0f },
			{ 3.0f, 5.0f, 0.0f },

			{ 2.0f, 5.0f, -1.0f },
			{ 2.0f, 5.0f, 1.0f },


		

			//Smaller Branch Leaves
			{ 0.0f, 4.0f, 1.0f },
			{ 0.0f, 3.0f, 1.0f },
			{ -1.0f, 4.0f, 0.0f },
			{ -1.0f, 3.0f, 0.0f },
			{ -1.0f, 2.0f, 0.0f },
			{ 0.0f, 2.0f, -1.0f },
			{ 1.0f, 1.0f, 0.0f },

		};

		this->parts.push_back(leafPart);

	}
	else if (TreeDisplayName == "Pine")
	{
		//Trunk
		TreePart trunkPart;
		trunkPart.treeBlock = trunkBlock;
		trunkPart.relativePos =
		{
			{0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 2.0f, 0.0f},
			{0.0f, 3.0f, 0.0f},
			{0.0f, 4.0f, 0.0f},
			{0.0f, 5.0f, 0.0f},
			{0.0f, 6.0f, 0.0f},
			{0.0f, 7.0f, 0.0f},
			{0.0f, 8.0f, 0.0f},
		};
		this->parts.push_back(trunkPart);


		//Leaf
		TreePart leafPart;
		leafPart.treeBlock = leafBlock;
		leafPart.relativePos =
		{
			//Center blocks
			{0.0f, 9.0f, 0.0f},
			{0.0f, 10.0f, 0.0f},

			//The very top
			{ 1.0f, 9.0f, 0.0f },
			{ -1.0f, 9.0f, 0.0f },
			{ 0.0f, 9.0f, -1.0f },
			{ 0.0f, 9.0f, 1.0f },

			//The Bottom Layer
			{ 0.0f, 3.0f, 1.0f },
			{ 0.0f, 3.0f, 2.0f },
			{ 0.0f, 3.0f, 3.0f },

			{ 0.0f, 3.0f, -1.0f },
			{ 0.0f, 3.0f, -2.0f },
			{ 0.0f, 3.0f, -3.0f },

			{ 1.0f, 3.0f, 0.0f },
			{ 2.0f, 3.0f, 0.0f },
			{ 3.0f, 3.0f, 0.0f },

			{ -1.0f, 3.0f, 0.0f },
			{ -2.0f, 3.0f, 0.0f },
			{ -3.0f, 3.0f, 0.0f },


			{ 1.0f, 3.0f, 1.0f },
			{ 1.0f, 3.0f, 2.0f },
			{ 2.0f, 3.0f, 1.0f },

			{ -1.0f, 3.0f, 1.0f },
			{ -1.0f, 3.0f, 2.0f },
			{ -2.0f, 3.0f, 1.0f },

			{ 1.0f, 3.0f, -1.0f },
			{ 1.0f, 3.0f, -2.0f },
			{ 2.0f, 3.0f, -1.0f },

			{ -1.0f, 3.0f, -1.0f },
			{ -1.0f, 3.0f, -2.0f },
			{ -2.0f, 3.0f, -1.0f },

			//Next "buffer" layer
			{ 1.0f, 4.0f, 0.0f },
			{ -1.0f, 4.0f, 0.0f },
			{ 0.0f, 4.0f, -1.0f },
			{ 0.0f, 4.0f, 1.0f },

			//Middle Layer

			{ 0.0f, 5.0f, 1.0f },
			{ 0.0f, 5.0f, 2.0f },

			{ 0.0f, 5.0f, -1.0f },
			{ 0.0f, 5.0f, -2.0f },

			{ 1.0f, 5.0f, 0.0f },
			{ 2.0f, 5.0f, 0.0f },

			{ -1.0f, 5.0f, 0.0f },
			{ -2.0f, 5.0f, 0.0f },


			{ 1.0f, 5.0f, 1.0f },
			{ -1.0f, 5.0f, 1.0f },
			{ 1.0f, 5.0f, -1.0f },
			{ -1.0f, 5.0f, -1.0f },

			//Next "buffer" layer
			{ 1.0f, 6.0f, 0.0f },
			{ -1.0f, 6.0f, 0.0f },
			{ 0.0f, 6.0f, -1.0f },
			{ 0.0f, 6.0f, 1.0f },

			{ 1.0f, 7.0f, 0.0f },
			{ -1.0f, 7.0f, 0.0f },
			{ 0.0f, 7.0f, -1.0f },
			{ 0.0f, 7.0f, 1.0f },
			

		};


		this->parts.push_back(leafPart);
	}


	else if (TreeDisplayName == "Fallen_Log")
	{
		//Trunk
		TreePart trunkPart;
		trunkPart.treeBlock = trunkBlock;
		trunkPart.relativePos =
		{
			//Trunk
			{1.0f, 0.0f, 0.0f},
			{2.0f, 0.0f, 0.0f},
			{3.0f, 0.0f, 0.0f},
			{4.0f, 0.0f, 0.0f},
			{5.0f, 0.0f, 0.0f}
		};
		this->parts.push_back(trunkPart);
	}
	else if (TreeDisplayName == "Stone_Rock")
	{
		//Trunk
		TreePart trunkPart;
		trunkPart.treeBlock = trunkBlock;
		trunkPart.relativePos =
		{
			//Rock Body
			{0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, -1.0f},

			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f}
		};
		this->parts.push_back(trunkPart);
	}
		
}