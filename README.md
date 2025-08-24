![Branding](https://github.com/TheMrSnoop/Voxl-Engine/blob/main/Images/Branding/TerracubeGradient.png)
My attempt at a custom made game engine.
Created in C++ to force myself to learn it, at least to an intermediate level.

# Still left to do
* Shadows
* Voxel Terrain
  
# Concept Art
How I would want the engine to look if I don't give up
![alt text](https://github.com/TheMrSnoop/Voxl-Engine/blob/main/Images/Voxl%20Concept.png)

# Engine Screenshots
Pixelation Shader
![alt text](https://github.com/TheMrSnoop/Voxl-Engine/blob/main/Images/pixels.png)

Black Outline Shader
![alt text](https://github.com/TheMrSnoop/Voxl-Engine/blob/main/Images/outline.png)

# PLANNED VOXL ENGINE TERRAIN PIPELINE
## The Goal
To generate a consistent, yet random voxel terrain generation. I will use a mixture of noise maps, and basic algorithms to accomplish this goal. 
All the terrain will be split into 16x16x32 block chunks.

## 01 | Terrain Generation
The engine will start off with a 16x16 chunk world, which is 256 blocks across. Using a heightmap, likely based on perlin noise. At this point, the entire landscape is made out of a single block, likely stone, and the Y axis difference can vary, depending on values multiplied to the noisemap. 

## 02 | Ground Generation
Now that the framework for the world has been created, the underground needs to be finalized. I shouldn’t spend much time trying to figure out how to do caves and such, so I will simply spawn ores in random patches/veins, which will depend on its Y-level. 

## 03 | Biome Generation
Generating biomes in the Voxl Engine is slightly different from Minecraft. Minecraft uses separate rainfall, and temperature maps to create a biomemap, which is then applied to the terrain.

In Voxl, the terrain height map IS the rainfall map. By simply stating that higher elevation = less rainfall, and lower elevation = more rainfall, I can avoid weird biome mixing found in minecraft. Essentially, mountains will always either be a cold deserted biome, or a hot deserted biome. Which I believe is more realistic than Minecraft’s jungle on a mountain range. Although there might be a reason why dozens of professional software engineers decided to go with their method, but I guess I’ll possibly have to learn that the hard way.

After generating this biomemap, and assigning (a small set) of biomes to the rainfall and temperature values, I can apply these biomes to the terrain, and start replacing the surface stone or whatever with the corresponding biome blocks. Whether it's desert sand, or forest grass.

## 04 | Structure Generation
I don’t really feel like going too crazy on “realistic” structure generation, so I’ll just scatter them randomly

## 05 | Foliage Generation
Now that we have a terrain, split into biomes, I can start spawning trees. The spawning of trees and foliage, is similar to the structure generation, where the actual position of the foliage doesn’t really matter much, but for foliage, the biome it is placed in determines the type of tree that is spawned. So if the biome is a taiga biome, it will spawn pine trees, if it's an oak forest biome, it will spawn oak trees. This also goes for foliage, and it would be kinda cool if I had a biome that only had different sized rocks spawn. 


