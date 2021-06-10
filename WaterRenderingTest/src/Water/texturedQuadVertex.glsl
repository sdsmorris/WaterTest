#version 330 core

layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec2 in_TexCoords;

out vec2 o_TexCoords;

void main()
{
	gl_Position = vec4(in_Pos, 1.0);
	o_TexCoords = in_TexCoords;
}