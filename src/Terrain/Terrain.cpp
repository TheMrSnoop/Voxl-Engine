#include "Terrain.h"
#include "Tree.h"
#include "Block.h"
#include "VoxlEngine.h"
#include "Texture.h"

#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> 

#include <unordered_map>

#include "Noise.h"
#include "UserInterface.h"

#include <core.h>


std::vector<Chunk::worldChunkData> allWorldChunkData;

int Chunk::ChunkWidth = 16;
int Chunk::ChunkHeight = 8;

//Initializes Chunk Variables
float Angle = 0.0f;
float Radius = 1.0f;

float Chunk_X_Position = 0.0f;
float Chunk_Z_Position = 0.0f;

int MaxSquares = 9;

bool GeneratedChunks = false;

using namespace std;


//this should eventually be tied to different biomes 
std::vector<std::string> PossibleTreeSpawns = { "Oak", "Oak_Large", "Pine" };



//atlas config
static const int ATLAS_SIZE_PX = 64;
static const int ATLAS_TILE_PX = 16;
static const int ATLAS_COLS = ATLAS_SIZE_PX / ATLAS_TILE_PX; // 4
static const int ATLAS_ROWS = ATLAS_COLS; //makes it a square

uint64_t TerrainGeneration::seed = VoxlEngine::generateSeed();



// file-scope pointer � initially null
Texture* terrainAtlasTexture = nullptr;

// call after GL context is created 
void Chunk::InitTerrainAtlas()
{
    //Initalizes the terrain
    if (terrainAtlasTexture == nullptr)
    {
        terrainAtlasTexture = new Texture("C:/dev/Voxl-Engine/Images/BlockTextures/TerrainAtlas.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
        // could be used for a chunk shader later
        // terrainAtlasTexture->texUnit(yourChunkShader, "uAtlas", 0); // where 0 = texture unit index for GL_TEXTURE0
    }

    //Prints the generated seed.
    cout << "World Seed: " << TerrainGeneration::seed << endl;
    VoxlEngine::Print("World Seed: " + std::to_string(TerrainGeneration::seed));
}



// only if padding seems to be needed
static const float ATLAS_PAD_UV = 0.0f;

struct TileUV { 
    float u0, v0, u1, v1; 
};


inline TileUV atlasUVFromIndex(int tileIndex)
{
    int col = tileIndex % ATLAS_COLS;
    int row = tileIndex / ATLAS_COLS;

    const float du = 1.0f / (float)ATLAS_COLS;
    const float dv = 1.0f / (float)ATLAS_ROWS;

    float u0 = col * du + ATLAS_PAD_UV;
    float v0 = row * dv + ATLAS_PAD_UV;
    float u1 = (col + 1) * du - ATLAS_PAD_UV;
    float v1 = (row + 1) * dv - ATLAS_PAD_UV;

    // NOTE: I might need to flip the texture on Y
    // float tmp_v0 = 1.0f - v1; float tmp_v1 = 1.0f - v0; v0 = tmp_v0; v1 = tmp_v1;

    return { u0, v0, u1, v1 };
}

inline void placeUV(const TileUV& r, float localU, float localV, float& outU, float& outV)
{
    outU = r.u0 + (r.u1 - r.u0) * localU;
    outV = r.v0 + (r.v1 - r.v0) * localV;
}


// blockID -> tile index map, is currently compltely mismatched
inline int getAtlasTileForBlockID(const std::string& id)
{
    static std::unordered_map<std::string, int> map = {
        {"Grass Top", 0},
        {"Dirt",  1},
        {"Stone", 2},
        {"Log", 3},
        {"Leaves", 4},
        {"Pine Log", 5},
        {"Pine Leaves", 6},
        {"Coal Ore", 7},
        {"Iron Ore", 8},
        {"Emerald Ore",9},
        {"Opal Ore", 10},
        {"Ruby Ore", 11},
        {"Gold Ore", 12},
        {"Sand", 13},
        {"Water", 14},
        {"Lava", 15}
    };

    auto it = map.find(id);
    if (it != map.end()) return it->second;
    return 0; // fallback tile (stone) if unknown
}


// FRONT (+Z), BACK (-Z), LEFT (-X), RIGHT (+X), TOP (+Y), BOTTOM (-Y)
glm::vec3 faceOffsets[6] = {
    { 0, 0, 1 },   // +Z front
    { 0, 0, -1 },  // -Z back
    { -1, 0, 0 },  // -X left
    { 1, 0, 0 },   // +X right
    { 0, 1, 0 },   // +Y top
    { 0, -1, 0 }   // -Y bottom
};


// indices for a single quad
unsigned int quadIdx[6] = { 0, 1, 2, 2, 3, 0 };

std::vector<GLfloat> vertices =
{
    // FRONT
    -0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,
     0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,
     0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,
    -0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,
    // BACK
     0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
     0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,
     // LEFT
     -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
     -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
     -0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
     -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,
     // RIGHT
      0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
      0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
      0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
      0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,
      // TOP
      -0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,
       0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,
       0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,
      -0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,
      // BOTTOM
      -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,
       0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,
       0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,
      -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f
};


bool doesThisChunkHaveABlockAtLocation(const glm::vec3& searchingPosition, const std::vector<Block>& blocks)
{
    for (const Block& block : blocks)
    {
        if (block.Position.x == searchingPosition.x && block.Position.y == searchingPosition.y  && block.Position.z == searchingPosition.z)
        {
            return true;
        }
    }
    return false;
}

// checks the surrounding chunks
bool isPositionOccupiedByAnyChunk(const glm::vec3& pos, const std::vector<Block>& thisChunkBlocks)
{
    //first it scans for itself
    if (doesThisChunkHaveABlockAtLocation(pos, thisChunkBlocks)) {
        return true;
    }

    // scans other chunks
    for (const Chunk::worldChunkData& wc : allWorldChunkData)
    {
        glm::vec3 chunkPos = wc.chunkPosition;
        float halfSize = Chunk::ChunkWidth * 1.0f;
        if (pos.x < chunkPos.x - halfSize - 1.0f || pos.x > chunkPos.x + halfSize + 1.0f) {
            continue;
        }
        if (pos.z < chunkPos.z - halfSize - 1.0f || pos.z > chunkPos.z + halfSize + 1.0f) {
            continue;
        }

        if (doesThisChunkHaveABlockAtLocation(pos, wc.chunkBlocks)) {
            return true;
        }
    }

    return false;
}


Chunk::ChunkMesh BuildChunkMesh(Shader& shaderProgram, const std::vector<Block>& blocks)
{
    Chunk::ChunkMesh mesh;
    mesh.vertices.clear();
    mesh.indices.clear();

    unsigned int indexOffset = 0;

    // loop blocks and builds visible faces
    for (const Block& block : blocks)
    {
        const glm::vec3 pos = block.Position;

        for (int face = 0; face < 6; ++face)
        {
            glm::vec3 neighborPos = pos + faceOffsets[face];
            if (isPositionOccupiedByAnyChunk(neighborPos, blocks)) continue; // hidden face

            int baseIndex = face * 4 * 8;

            // atlas handling (per-block)
            int tileIndex = getAtlasTileForBlockID(block.blockID);
            TileUV rect = atlasUVFromIndex(tileIndex);

            // push four vertices for this face
            for (int v = 0; v < 4; ++v)
            {
                float vx = vertices[baseIndex + v * 8 + 0] + pos.x;
                float vy = vertices[baseIndex + v * 8 + 1] + pos.y;
                float vz = vertices[baseIndex + v * 8 + 2] + pos.z;

                float cr = vertices[baseIndex + v * 8 + 3];
                float cg = vertices[baseIndex + v * 8 + 4];
                float cb = vertices[baseIndex + v * 8 + 5];

                float localU = vertices[baseIndex + v * 8 + 6];
                float localV = vertices[baseIndex + v * 8 + 7];

                float finalU, finalV;
                placeUV(rect, localU, localV, finalU, finalV);

                mesh.vertices.push_back(vx);
                mesh.vertices.push_back(vy);
                mesh.vertices.push_back(vz);

                mesh.vertices.push_back(cr);
                mesh.vertices.push_back(cg);
                mesh.vertices.push_back(cb);

                mesh.vertices.push_back(finalU);
                mesh.vertices.push_back(finalV);
            }

            // adds indices for the quad (two triangles)
            mesh.indices.push_back(indexOffset + 0);
            mesh.indices.push_back(indexOffset + 1);
            mesh.indices.push_back(indexOffset + 2);
            mesh.indices.push_back(indexOffset + 2);
            mesh.indices.push_back(indexOffset + 3);
            mesh.indices.push_back(indexOffset + 0);

            indexOffset += 4;
        }
    }


    mesh.indexCount = (int)mesh.indices.size();
    if (mesh.indexCount == 0)
    {
        return mesh;
    }

    // uploads to GPU
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));               // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // uv
    glEnableVertexAttribArray(2);

    // unbinds VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::cout << VoxlEngine::returnConsoleBar((((allWorldChunkData.size() + 1) * 1.0f) / 9), "Generating Terrain") << std::endl;

    //Canvas TerrainMenu(ImVec2(20, 20), ImVec2(800, 800), "GENERATING TERRAIN");

    //TextBlock txt_terrainGen(TerrainMenu, fmt::format("Generating Terrain: {:1}%", round((((allWorldChunkData.size() + 1))))));

    //Canvas::Render(TerrainMenu);

    //std::cout << "Generating Terrain: " << round((((allWorldChunkData.size() + 1) * 1.0f) / 27) * 100) << "%" << std::endl;


    return mesh;
}


Chunk::Chunk(glm::vec3 inputCordinates, std::vector<Block> inputBlockVector, Shader shaderProgram)
{
    cordinates = inputCordinates;
    // chunk data is stored in allWorldChunkData
}


// convert degrees to radians
float AnglesToRadians(float Angle)
{
    const double PI = 3.14159265358979323846;
    return (Angle * (PI / 180.0));
}


// draws chunk meshes
void RenderAllChunks(Shader& shaderProgram)
{
    shaderProgram.Activate();

    // builds missing meshes 
    for (Chunk::worldChunkData& thisChunkData : allWorldChunkData)
    {
        if (thisChunkData.mesh.VAO == 0)
        {
            thisChunkData.mesh = BuildChunkMesh(shaderProgram, thisChunkData.chunkBlocks);
        }
    }

    for (Chunk::worldChunkData& thisChunkData : allWorldChunkData)
    {
        if (thisChunkData.mesh.VAO == 0 || thisChunkData.mesh.indexCount == 0) continue;

        int texIdx = thisChunkData.mesh.textureIndex;
        
        terrainAtlasTexture->Bind();

        shaderProgram.setMat4("model", glm::mat4(1.0f));

        glBindVertexArray(thisChunkData.mesh.VAO);
        glDrawElements(GL_TRIANGLES, thisChunkData.mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}



std::vector<Block> allNewChunkBlocks;

std::vector<glm::vec3> allTreePositions;

void GenerateTrees(int Count)
{
    for (int i = 0; i < Count; i++)
    {
        //Tree Spawn Rates
        //OAK: 50%
        //LARGE OAK: 50%
        Tree newTree = (VoxlEngine::getRandomInt(0, 1) == 0) ? Tree::ReturnTree("Oak") : Tree::ReturnTree("Oak_Large");


        

        glm::vec3 treeSpawnPosition = glm::vec3(Chunk_X_Position + (float)VoxlEngine::getRandomInt(-8, 8), 15.0f, Chunk_Z_Position + (float)VoxlEngine::getRandomInt(-8, 8));

        //there is only 2 parts, so this should happen twice.
        for (Tree::TreePart part : newTree.parts)
        {
            for (glm::vec3 logPositions : part.relativePos)
            {
                //remember, textures are still mismatched.
                Block newChunkBlock(part.treeBlock.DisplayName, logPositions + treeSpawnPosition);

                allNewChunkBlocks.push_back(newChunkBlock);
            }
        }
    }
}

void GenerateWater()
{
    glm::vec3 relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f);

    // Spawns a column of stone blocks in a row (Z Axis)
    for (int z = Chunk::ChunkWidth * -1; z < Chunk::ChunkWidth; z++)
    {
        //Spawns a column of blocks in a row (X Axis)
        for (int x = Chunk::ChunkWidth * -1; x < Chunk::ChunkWidth; x++)
        {
            int Y_Min_Height;
            int Y_Max_Height;


            //spawns a column of blocks, normally.
            for (int y = 10; y < 16; y++)
            {
                relativeOffset = glm::vec3(x * 1.0f, 1.0f, z * 1.0f);

                if (!doesThisChunkHaveABlockAtLocation(relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position), allNewChunkBlocks))
                {
                    //remember, textures are still mismatched.
                    Block newChunkBlock("Stone", relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                    allNewChunkBlocks.push_back(newChunkBlock);
                }
            }
        }
    }
}

void GenerateChunkLayer(std::string BlockID, int minHeight, glm::vec2 maxHeightRange, bool isSurfaceLayer)
{
    glm::vec3 relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f);


    // Spawns a column of stone blocks in a row (Z Axis)
    for (int z = Chunk::ChunkWidth * -1; z < Chunk::ChunkWidth; z++)
    {
        //Spawns a column of blocks in a row (X Axis)
        for (int x = Chunk::ChunkWidth * -1; x < Chunk::ChunkWidth; x++)
        {
            int Y_Min_Height;
            int Y_Max_Height;

            if (!isSurfaceLayer)
            {
                Y_Min_Height = maxHeightRange.x;
                Y_Max_Height = maxHeightRange.y;

                //spawns a column of blocks, normally.
                for (int y = 0; y < VoxlEngine::getRandomInt(maxHeightRange.x, maxHeightRange.y); y++)
                {
                    relativeOffset = glm::vec3(x * 1.0f, 1.0f, z * 1.0f);

                    //if true, then the chunk already has a block at location, so I inverse it with !.
                    //speeds up chunk generation by about 1.75x, because it only adds a new block if it doesnt already exsist.
                    if (!doesThisChunkHaveABlockAtLocation(relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position), allNewChunkBlocks))
                    {
                        Block newChunkBlock(BlockID, relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                        allNewChunkBlocks.push_back(newChunkBlock);
                    }
                }
            }
            else
            {
                //Noise DATA
                float freq = 0.1f;          // big flat areas -> lower; more detail -> higher
                float amplitude = 1.0f;      // �1 block typical; use 2.0 for �2 blocks
                int quantizeLevels = 2;      // 1 = continuous; 2 = stronger plateaus

                int baseMax = (int)maxHeightRange.y; // your intended base height

                int minClamp = maxHeightRange.y - 1;
                int maxClamp = maxHeightRange.y + 20; // or any world cap you use

                Y_Max_Height = PerlinNoise::computeHeightFromPerlinColumn(baseMax, Chunk_X_Position + x, Chunk_Z_Position + z,
                    freq, amplitude, quantizeLevels, TerrainGeneration::seed,
                    minClamp, maxClamp);


                //spawns a collumn of blocks using perlin noise
                for (int y = 0; y < Y_Max_Height; y++)
                {
                    relativeOffset = glm::vec3(x * 1.0f, 1.0f, z * 1.0f);
                    //speeds up chunk generation by about 1.75x, because it only adds a new block if it doesnt already exsist.
                    if (!doesThisChunkHaveABlockAtLocation(relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position), allNewChunkBlocks))
                    {
                        Block newChunkBlock(BlockID, relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                        allNewChunkBlocks.push_back(newChunkBlock);

                        //Should only generate trees (sometimes) on the highest block, on the surface.
                    }
                }
            }
        }
    }
}


void Chunk::SpawnChunks(glm::uint Iterations, Shader shaderProgram)
{
    if (!GeneratedChunks)
    {
        for (int RingIndex = 0; RingIndex <= Iterations; RingIndex++)
        {
            MaxSquares = (RingIndex * 9) - RingIndex;
            Radius = RingIndex * 1.0f;

            Angle = 0.0f;

            //Creates chunks in a radial pattern
            for (int i = 0; i <= MaxSquares; i++)
            {
                Chunk_X_Position = std::sin(AnglesToRadians(Angle)) * Radius;
                Chunk_Z_Position = std::cos(AnglesToRadians(Angle)) * Radius;

                Chunk_X_Position = std::round(Chunk_X_Position) * (ChunkWidth * 2.0f);
                Chunk_Z_Position = std::round(Chunk_Z_Position) * (ChunkWidth * 2.0f);

                float Percentage = (i * 1.0f) / (MaxSquares * 1.0f);
                Angle = (360.0f * Percentage);

                glm::vec3 relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f);

                
                //Spawns the chunk by layer
                //I know the ID doesnt make any sense, but the atlas texture array or smth is mismatched with the block database...
                // 
                //Stone Layer
                GenerateChunkLayer("Water", 0, glm::vec2(9, 10), false);

                //Dirt Layer
                GenerateChunkLayer("Sand", 11, glm::vec2(11, 13), false);

                //Grass Layer
                GenerateChunkLayer("Gold Ore", 13, glm::vec2(-1, 15), true);

                //GenerateWater();

                GenerateTrees(VoxlEngine::getRandomInt(1, 2));



                // avoids rebuilding every frame
                Chunk::worldChunkData worldData;
                worldData.chunkPosition = glm::vec3(Chunk_X_Position, 0.0f, Chunk_Z_Position); 
                worldData.chunkBlocks = std::move(allNewChunkBlocks);

                // build mesh and store inside worldData
                worldData.mesh = BuildChunkMesh(shaderProgram, worldData.chunkBlocks);


                allWorldChunkData.push_back(std::move(worldData));

                
                for (Chunk::worldChunkData& wc : allWorldChunkData)
                {
                    if (wc.mesh.VAO == 0) // not yet built
                    {
                        wc.mesh = BuildChunkMesh(shaderProgram, wc.chunkBlocks);
                    }
                }
            }
        }

        GeneratedChunks = true;
        VoxlEngine::Print("All Chunks have been generated!");
    }
    else
    {
        RenderAllChunks(shaderProgram); 
    }
}

int Chunk::returnChunk(glm::vec3 position)
{
    for (int i = 0; i < allWorldChunkData.size() - 1; i++)
    {
        if (allWorldChunkData[i].chunkPosition.x == position.x && allWorldChunkData[i].chunkPosition.y == position.y && allWorldChunkData[i].chunkPosition.z == position.z)
        {
            return i;
        }
    }
    return -1;
}