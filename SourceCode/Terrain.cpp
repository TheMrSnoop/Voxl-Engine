#include "Terrain.h"
#include<cmath>
#include <iostream>

#include <GLFW/glfw3.h>

std::vector<glm::vec2> chunks;

float Angle = 0.0f;
float Radius = 1.0f;

float Chunk_X_Position = 0.0f;
float Chunk_Y_Position = 0.0f;

int MaxSquares = 9;

bool GeneratedChunks = false;


Chunk::Chunk(glm::vec3 inputCordinates, Shader shaderProgram)
{
	cordinates = inputCordinates;
	
	Block::SpawnAreaOfBlocks("Grass Top", cordinates, glm::vec2(-8.0f, 8.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-8.0f, 8.0f), shaderProgram);
}

float AnglesToRadians(float Angle)
{
    const double PI = 3.14159265358979323846;

    return (Angle * (PI / 180.0));
}

void RenderAllChunks(Shader shaderProgram)
{
    for (glm::vec2 ChunkPosition : chunks)
    {
        Chunk NewChunk(glm::vec3(ChunkPosition.x, -1.0f, ChunkPosition.y), shaderProgram);
    }
}

void Chunk::SpawnChunks(glm::uint Iterations, Shader shaderProgram)
{
    if (!GeneratedChunks)
    {
        for (int RingIndex = 1; RingIndex <= Iterations; RingIndex++)
        {
            MaxSquares = (RingIndex * 9) - RingIndex;
            Radius = RingIndex * 1.0f;

            Angle = 0.0f;

            for (int i = 1; i <= MaxSquares; i++)
            {
                //std::sin and std::cos only takes Radians, so I am coverting from Degrees, cuz idk how Radians really works.
                Chunk_X_Position = std::sin(AnglesToRadians(Angle)) * Radius;
                Chunk_Y_Position = std::cos(AnglesToRadians(Angle)) * Radius;

                Chunk_X_Position = std::round(Chunk_X_Position) * 16.0f;
                Chunk_Y_Position = std::round(Chunk_Y_Position) * 16.0f;

                //Used for debugging
                std::cout << "Angle: " << Angle << ", MaxSquares: " << MaxSquares << std::endl;
                std::cout << "Adding chunk at " << Chunk_X_Position << ", " << Chunk_Y_Position << std::endl << std::endl;

                chunks.push_back({ Chunk_X_Position, Chunk_Y_Position });

                float Percentage = (i * 1.0f) / (MaxSquares * 1.0f);

                Angle = (360.0f * Percentage);
            }
        }

        GeneratedChunks = true;
    }
    else
    {
        RenderAllChunks(shaderProgram);
    }
}