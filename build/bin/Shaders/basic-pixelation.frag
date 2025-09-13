#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform float pixelSize; // add this to control pixelation level
uniform vec2 textureSize; // pass the texture size in pixels (e.g. 64x64)

void main()
{
    // pixelate coords before sampling texture
    vec2 pixelatedUV = floor(texCoord * textureSize / pixelSize) * pixelSize / textureSize;

    FragColor = texture(tex0, pixelatedUV);

    if (FragColor.a < 0.1)
        discard;
}
