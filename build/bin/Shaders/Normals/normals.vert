#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;

out vec3 vNormal;



void main()
{
    vec3 p = aPos - vec3(0.5);
    vec3 absPos = abs(p);


    if (absPos.x > absPos.y && absPos.x > absPos.z)
        vNormal = vec3(sign(aPos.x), 0.0, 0.0);
    else if (absPos.y > absPos.x && absPos.y > absPos.z)
        vNormal = vec3(0.0, sign(aPos.y), 0.0);
    else
        vNormal = vec3(0.0, 0.0, sign(aPos.z));

    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}
