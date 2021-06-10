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

uniform vec4 u_ClipPlane;

void main()
{
    //Pass tex coords to fragment shader
    TexCoords = inTexCoords;    
    //Calc world position
    vec4 pos = model * vec4(inPos, 1.0);
    //Set vertex pos
    gl_Position = projection * view * pos;
    //Set clip plane to cull vertices
    gl_ClipDistance[0] = dot(pos, u_ClipPlane);

    //FOG
    vec4 posRelativeToCam = view * model * vec4(inPos, 1.0);
    float distance = length(posRelativeToCam.xyz);
    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}