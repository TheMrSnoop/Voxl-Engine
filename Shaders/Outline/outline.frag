#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    float threshold = 0.01;

    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
    float depth = texture(screenTexture, TexCoord).r;

    // sample neighbors
    float depthLeft  = texture(screenTexture, TexCoord + vec2(-texelSize.x, 0)).r;
    float depthRight = texture(screenTexture, TexCoord + vec2(texelSize.x, 0)).r;
    float depthUp    = texture(screenTexture, TexCoord + vec2(0, texelSize.y)).r;
    float depthDown  = texture(screenTexture, TexCoord + vec2(0, -texelSize.y)).r;

    float diff = 0.0;
    diff += abs(depth - depthLeft);
    diff += abs(depth - depthRight);
    diff += abs(depth - depthUp);
    diff += abs(depth - depthDown);

    if(diff > threshold)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // black outline
    else
        FragColor = texture(screenTexture, TexCoord);
}
