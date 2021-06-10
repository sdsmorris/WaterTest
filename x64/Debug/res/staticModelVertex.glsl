#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

//FOG
const float density = 0.35;
const float gradient = 5.0;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = inTexCoords;    
    gl_Position = projection * view * model * vec4(inPos, 1.0);

    //FOG
    vec4 posRelativeToCam = view * model * vec4(inPos, 1.0);
    float distance = length(posRelativeToCam.xyz);
    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}