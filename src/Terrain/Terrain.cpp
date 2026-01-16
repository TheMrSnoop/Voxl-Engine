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

uint64_t TerrainGeneration::seed = 0; //VoxlEngine::generateSeed();



// file-scope pointer, initially null
Texture* terrainAtlasTexture = nullptr;

static std::vector<Texture::atlasData> atlasMap;
void updateAtlasMap()
{
    json atlasJson = VoxlEngine::loadJson("../../src/Terrain/terrainAtlas.json");

    for (const auto& item : atlasJson["atlasMap"])
    {
        //Fills out vector array using json data.
        atlasMap.push_back({item["id"].get<std::string>(), item["blockTextures"].get<std::vector<int>>()});
    }
}

// call after GL context is created 
void Chunk::InitTerrainAtlas()
{
    //Initalizes the terrain
    if (terrainAtlasTexture == nullptr)
    {
        terrainAtlasTexture = new Texture("C:/dev/Voxl-Engine/Images/BlockTextures/TerrainAtlas.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, false);
        // could be used for a chunk shader later
        // terrainAtlasTexture->texUnit(yourChunkShader, "uAtlas", 0); // where 0 = texture unit index for GL_TEXTURE0
    }

    //Prints the generated seed.
    cout << "World Seed: " << TerrainGeneration::seed << endl;
    VoxlEngine::Print("World Seed: " + std::to_string(TerrainGeneration::seed));

    updateAtlasMap();
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
int getAtlasTileForBlockID(std::string id, int faceIndex)
{
    for (Texture::atlasData element : atlasMap)
    {
        if (element.ID == id)
        {
            return element.textureArray[faceIndex];
        }
    }

    //return grass by default
    return 0;
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

glm::vec3 faceNormals[6] = {
    { 0, 0, 1 },   // +Z
    { 0, 0, -1 },  // -Z
    { -1, 0, 0 },  // -X
    { 1, 0, 0 },   // +X
    { 0, 1, 0 },   // +Y
    { 0, -1, 0 }   // -Y
};



// indices for a single quad
unsigned int quadIdx[6] = { 0, 1, 2, 2, 3, 0 };

// Cube geometry 
std::vector<GLfloat> vertices =
{
    // FRONT
    -0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,  0.0f,0.0f,1.0f,
    0.5f, -0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,  0.0f,0.0f,1.0f,
    0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,  0.0f,0.0f,1.0f,
    -0.5f,  0.5f, 0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,  0.0f,0.0f,1.0f,

    // BACK
    0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,  0.0f,0.0f,-1.0f,
    -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,  0.0f,0.0f,-1.0f,
    -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,  0.0f,0.0f,-1.0f,
    0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,  0.0f,0.0f,-1.0f,

    // LEFT
    -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f, -1.0f,0.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f, -1.0f,0.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f, -1.0f,0.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f, -1.0f,0.0f,0.0f,

    // RIGHT
    0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,  1.0f,0.0f,0.0f,
    0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,  1.0f,0.0f,0.0f,
    0.5f,  0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,  1.0f,0.0f,0.0f,
    0.5f,  0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,  1.0f,0.0f,0.0f,

    // TOP
    -0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    0.0f,0.0f,  0.0f,1.0f,0.0f,
    0.5f, 0.5f,  0.5f,   0.8f,0.7f,0.4f,    1.0f,0.0f,  0.0f,1.0f,0.0f,
    0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    1.0f,1.0f,  0.0f,1.0f,0.0f,
    -0.5f, 0.5f, -0.5f,   0.8f,0.7f,0.4f,    0.0f,1.0f,  0.0f,1.0f,0.0f,

    // BOTTOM
    -0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    0.0f,0.0f,  0.0f,-1.0f,0.0f,
    0.5f, -0.5f, -0.5f,  0.8f,0.7f,0.4f,    1.0f,0.0f,  0.0f,-1.0f,0.0f,
    0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    1.0f,1.0f,  0.0f,-1.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.8f,0.7f,0.4f,    0.0f,1.0f,  0.0f,-1.0f,0.0f
};







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

            
            

            constexpr int FLOATS_PER_VERTEX = 11;
            constexpr int VERTS_PER_FACE = 4;

            int baseIndex = face * VERTS_PER_FACE * FLOATS_PER_VERTEX;

            // atlas handling (per-block)
            int tileIndex;
            tileIndex = getAtlasTileForBlockID(block.blockID, face);

            TileUV rect = atlasUVFromIndex(tileIndex);

            // push four vertices for this face
            for (int v = 0; v < 4; ++v)
            {
                glm::vec3 normal = faceNormals[face];

                int o = baseIndex + v * FLOATS_PER_VERTEX;


                float vx = vertices[o + 0] + pos.x;
                float vy = vertices[o + 1] + pos.y;
                float vz = vertices[o + 2] + pos.z;


                float cr = vertices[o + 3];
                float cg = vertices[o + 4];
                float cb = vertices[o + 5];

                float localU = vertices[o + 6];
                float localV = vertices[o + 7];

                // normals are already in the template
                float nx = vertices[o + 8];
                float ny = vertices[o + 9];
                float nz = vertices[o + 10];


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
                
                mesh.vertices.push_back(nx);
                mesh.vertices.push_back(ny);
                mesh.vertices.push_back(nz);
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

    GLsizei stride = 11 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                  // pos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // uv
    glEnableVertexAttribArray(2);

    glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);


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
        Tree newTree = (VoxlEngine::getRandomInt(0, 1) == 0) ? Tree::ReturnTree("Oak") : Tree::ReturnTree("Pine");


        

        glm::vec3 treeSpawnPosition = glm::vec3(Chunk_X_Position + (float)VoxlEngine::getRandomInt(-8, 8), 46.0f, Chunk_Z_Position + (float)VoxlEngine::getRandomInt(-8, 8));

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

                Block newChunkBlock("Stone", relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                allNewChunkBlocks.push_back(newChunkBlock);
            }
        }
    }
}

void GenerateChunkLayer(std::string BlockID, int minHeight, glm::vec2 maxHeightRange, bool isSurfaceLayer)
{
    glm::vec3 relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f);

    int CHUNK_X = Chunk::ChunkWidth * -1;
    int CHUNK_Z = Chunk::ChunkWidth * -1;


    bool mask[CHUNK_X][CHUNK_Z];
    int tile[CHUNK_X][CHUNK_Z];


    // Spawns a column of blocks in a row (Z Axis)
    for (int z = CHUNK_X; z < Chunk::ChunkWidth; z++)
    {
        //Spawns a column of blocks in a row (X Axis)
        for (int x = CHUNK_X; x < Chunk::ChunkWidth; x++)
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
                    Block newChunkBlock(BlockID, relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                    allNewChunkBlocks.push_back(newChunkBlock);
                }
            }
            else
            {
                //Noise DATA
                float freq = 0.115f;          // big flat areas -> lower; more detail -> higher
                float amplitude = 2.5f;      // 1 block typical; use 2.0 for 2 blocks
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
                    Block newChunkBlock(BlockID, relativeOffset + glm::vec3(Chunk_X_Position, y * 1.0f, Chunk_Z_Position));

                    allNewChunkBlocks.push_back(newChunkBlock);

                    //Should only generate trees (sometimes) on the highest block, on the surface.
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
                Chunk_X_Position = std::cos(AnglesToRadians(Angle)) * Radius;
                Chunk_Z_Position = std::sin(AnglesToRadians(Angle)) * Radius;

                Chunk_X_Position = std::round(Chunk_X_Position) * (ChunkWidth * 2.0f);
                Chunk_Z_Position = std::round(Chunk_Z_Position) * (ChunkWidth * 2.0f);

                float Percentage = (i * 1.0f) / (MaxSquares * 1.0f);
                Angle = (360.0f * Percentage);

                glm::vec3 relativeOffset = glm::vec3(0.0f, 0.0f, 0.0f);

                
                //Spawns the chunk by layer


                //Deepslate Layer
                GenerateChunkLayer("Deepslate", 0, glm::vec2(19, 20), true);

                //Stone Layer
                GenerateChunkLayer("Stone", 21, glm::vec2(31, 41), true);

                //Dirt Layer
                GenerateChunkLayer("Dirt", 41, glm::vec2(42, 44), true);

                //Grass Layer
                GenerateChunkLayer("Grass", 44, glm::vec2(42, 46), true);

                //GenerateWater();

                GenerateTrees(VoxlEngine::getRandomInt(1, 4));



                // avoids rebuilding every frame
                Chunk::worldChunkData worldData;
                worldData.chunkPosition = glm::vec3(Chunk_X_Position, 0.0f, Chunk_Z_Position); 
                worldData.chunkBlocks = std::move(allNewChunkBlocks);

                // build mesh and store inside worldData
                worldData.mesh = BuildChunkMesh(shaderProgram, worldData.chunkBlocks);


                allWorldChunkData.push_back(std::move(worldData));

                
                
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