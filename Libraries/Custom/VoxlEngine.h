#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <random>

class VoxlEngine
{
public:
	VoxlEngine();

	//VECTORS
	glm::vec2 getDisplayResolution();
	glm::vec3 getForwardVector();
	glm::vec3 multiplyVectorWithFloat(glm::vec3, float);
	static bool isVectorEqual(glm::vec3 vec1, glm::vec3 vec2);

	//RANDOMNESS
	static int getRandomInt(int min, int max);

	//only works with strings. I gotta make more, or a better single one.
	static std::string getRandomVectorMember_STR(std::vector<std::string> inputVector);

	//generates the world seed
	static uint64_t generateSeed();

	
	
	//STRINGS
	static std::string returnConsoleBar(float percentage, std::string Label = "");

	
	//ENGINE METRICS
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	static float DTPS;
	static float FPS;
	int roundedFPS = 0;
	void CalculateFPS();

	//GLOBAL ENGINE DATA
	static bool engineClosed;
	static bool showEngineMetrics;

	static std::string ConsoleText;
	static void Print(std::string text);

	static const int windowWidth = 2560;
	static const int windowHeight = 1440;

	static enum TabModes
	{
		World,
		TextureEditor,
		VoxelCreator,
		CharacterCreator
	};

	static TabModes currentTabMode;


	struct RaycastHit
	{
		glm::vec3 blockHitPosition;
		glm::vec3 hitPosition;
		glm::vec3 hitNormal;
		bool hit;
	};

	RaycastHit Raycast(glm::vec3 origin, glm::vec3 dir, int maxSteps);
private:
	//Also only for calculating FPS and DeltaTime
	float lastTime = 0.0f;
	float timeAccumulator = 0.0f;
	float delay = 1.0f;
};