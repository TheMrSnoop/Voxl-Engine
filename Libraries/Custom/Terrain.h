#pragma once
#include "Block.h"

class Chunk
{
public:

	//All of this is for the visual chunk
	Chunk(glm::vec3 inputCordinates, std::vector<Block> inputBlockVector, Shader shaderProgram);

	static void SpawnChunks(glm::uint Iterations, Shader shaderProgram);

	static int returnChunk(glm::vec3 position);

	static void InitTerrainAtlas();

	glm::vec3 cordinates;

	static int ChunkWidth;
	static int ChunkHeight;

	struct ChunkMesh
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		GLuint VAO = 0, VBO = 0, EBO = 0;
		int indexCount = 0;
		int textureIndex = -1; // index into Block::allBlockTextures (for now use first block's texture)
		std::vector<int> textureIndexArray;
	};

	//apparently I need to drop the static keyword.
	struct worldChunkData
	{
		glm::vec3 chunkPosition;
		std::vector<Block> chunkBlocks;

		ChunkMesh mesh;
	};

};

class TerrainGeneration
{
public:

	//Noise DATA

	//for some reason, const float doesnt work, so I need to do constexpr float instead.
	//const ints work though...?

	static constexpr float freq = 10.0f;          // big flat areas -> lower; more detail -> higher
	static constexpr float amplitude = 20.0f;      // �1 block typical; use 2.0 for �2 blocks
	static const int quantizeLevels = 1;      // 1 = continuous; 2 = stronger plateaus
	static uint64_t seed;          // global seed if you want determinism
};