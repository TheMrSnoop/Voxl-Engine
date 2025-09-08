#include"Tree.h"
#include<vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include <random>

Tree Tree::ReturnTree(const std::string TreeDisplayName)
{
	//Texture mismatch
	Tree newTree("Oak", Block::ReturnBlock("Lava"), Block::ReturnBlock("Iron Ore"));
	return newTree;
}

Tree::Tree(const std::string TreeDisplayName, const Block::BlockData& trunkBlock, const Block::BlockData& leafBlock)
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