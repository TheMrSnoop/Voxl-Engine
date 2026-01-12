#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    float outlineScale = 1.05; // 5% outline
    vec4 pos = model * vec4(aPos * outlineScale, 1.0);
    gl_Position = camMatrix * pos;
}
