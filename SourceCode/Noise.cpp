#include "Noise.h"


//PERLIN NOISE

glm::vec2 randomGradient(int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    glm::vec2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}


float dotGridGradient(int ix, int iy, float x, float y)
{
    glm::vec2 gradient = randomGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    return (dx * gradient.x + dy * gradient.y);
}

static inline float fadef(float t) {
    // classic Perlin fade: 6t^5 - 15t^4 + 10t^3
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}
static inline float lerpf(float a, float b, float t) { return a + t * (b - a); }

float PerlinNoise::Perlin(float x, float y)
{
    // grid cell coords
    int x0 = (int)std::floor(x);
    int y0 = (int)std::floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;


    float n00 = dotGridGradient(x0, y0, x, y);
    float n10 = dotGridGradient(x1, y0, x, y);
    float n01 = dotGridGradient(x0, y1, x, y);
    float n11 = dotGridGradient(x1, y1, x, y);

    float u = fadef(sx);
    float v = fadef(sy);

    float ix0 = lerpf(n00, n10, u);
    float ix1 = lerpf(n01, n11, u);
    float value = lerpf(ix0, ix1, v);

    return value; 
}



//compute column height from Perlin heightmap
int PerlinNoise::computeHeightFromPerlinColumn(int baseMaxY, float chunkWorldX, float chunkWorldZ, 
    float freq, float amplitude, 
    int quantizeLevels, uint32_t seed, 
    int minClamp, int maxClamp)
{
    // sample at scaled world coords
    float sampleX = chunkWorldX * freq;
    float sampleZ = chunkWorldZ * freq;

    float n = PerlinNoise::Perlin(sampleX, sampleZ);

    // helps create plateus
    // Quantizing just means restricting the "amplitude" to a set of values.
    if (quantizeLevels > 1)
    {
        // quantize by rounding to N discrete steps
        n = std::round(n * (float)quantizeLevels) / (float)quantizeLevels;
    }

    float deltaF = n * amplitude;        
    int delta = (int)std::round(deltaF);  

    int columnH = baseMaxY + delta;

    if (columnH < minClamp) columnH = minClamp;
    if (columnH > maxClamp) columnH = maxClamp;

    return columnH;
}
