#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D tex0;

uniform float ambientStrength;

//spotlight
uniform vec3 lightPos;
uniform vec3 lightColor; 

//sunlight
uniform vec3 sunDirection;
uniform vec3 sunColor;


void main()
{
    //texture color
    vec4 texColor = texture(tex0, texCoord);
    if (texColor.a < 0.1)
        discard;

    //lighting
    vec3 norm = normalize(normal);
    //vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, sunDirection), 0.0);
    vec3 diffuse = diff * sunColor;

    //Applies the light to the texture
    vec3 ambient = ambientStrength * sunColor;
    vec3 lighting = ambient + diffuse;
    vec3 finalColor = texColor.rgb * lighting;

    FragColor = vec4(finalColor, texColor.a);
}
