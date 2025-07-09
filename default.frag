#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;

uniform vec3 sunDirection;  // usually normalized
uniform vec3 sunColor;
uniform float sunStrength;


uniform vec3 lightPos;     // position of light-emitting block
uniform vec3 lightColor;   // color of light
uniform float lightPower;  // light strength

void main()
{
    vec4 texColor = texture(tex0, texCoord);
    if (texColor.a < 0.1)
        discard;

    float dist = length(lightPos - fragPos);
    float intensity = lightPower / (dist * dist); // simple fade

    // simulate normals (assuming top-down light and flat faces)
    vec3 normal = vec3(0.0, 1.0, 0.0); // y-up normal, tweak per face if u want

    float diff = max(dot(normal, -sunDirection), 0.0); // sun hits top surface

    vec3 lighting = sunColor * diff * sunStrength;

    //Applies the light to the texture

    vec3 finalColor = texColor.rgb * (color + lighting);
    FragColor = vec4(finalColor, texColor.a);

}
