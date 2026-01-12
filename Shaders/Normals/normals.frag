#version 330 core

in vec3 vNormal;
out vec4 FragColor;

void main()
{
    // visualize normals directly
    FragColor = vec4(normalize(vNormal) * 0.5 + 0.5, 1.0);
}
