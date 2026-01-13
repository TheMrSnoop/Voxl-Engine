#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float pixelSize;
uniform vec2 resolution;
uniform float outlineThickness;

void main()
{
    // pixelation logic
    vec2 pixelCoords = TexCoords * resolution;
    pixelCoords = floor(pixelCoords / pixelSize) * pixelSize;
    vec2 snappedUV = pixelCoords / resolution;

    vec3 centerColor = texture(screenTexture, snappedUV).rgb;

    // sobel edge detection kernel
    float edgeStrength = 10.0;
    float offset = pixelSize / resolution.x; // adjust for resolution

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2(0.0f,     offset), // top-center
        vec2(offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2(0.0f,     0.0f),   // center
        vec2(offset,   0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset, -offset)  // bottom-right
    );

    float kernelX[9] = float[](
        -1,  0, 1,
        -2,  0, 2,
        -1,  0, 1
    );

    float kernelY[9] = float[](
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    );

    float gx = 0.0;
    float gy = 0.0;

    for(int i = 0; i < 9; i++) {
        vec3 sample = texture(screenTexture, snappedUV + offsets[i]).rgb;
        float gray = dot(sample, vec3(0.299, 0.587, 0.114)); // grayscale
        gx += gray * kernelX[i];
        gy += gray * kernelY[i];
    }

    edgeStrength = sqrt(gx * gx + gy * gy) * outlineThickness; //! TEMPORARY REMOVING OUTLINE !

    // outline threshold
    if (edgeStrength > 0.2)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // black outline
    else
        FragColor = vec4(centerColor, 1.0);
}
