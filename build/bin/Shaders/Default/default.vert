#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 fragPos;
out vec3 color;
out vec2 texCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    color = aColor;
    texCoord = aTex;

    //! Inversing matrices is a costly operation for shaders, so wherever possible try to avoid doing inverse operations since they have to be done on each vertex of your scene. 
    //For an efficient application you'll likely want to calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing (just like the model matrix).

    normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}
