#version 330 core

layout (location = 0) in vec3 in_Pos;
//layout (location = 1) in vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 o_ClipSpace;

out vec2 o_TexCoords;

const float tiling = 6.0;

void main()
{
	gl_ClipDistance[0] = 1;
	vec4 pos = model * vec4(in_Pos, 1.0);
	o_ClipSpace = projection * view * model * vec4(in_Pos, 1.0);
    gl_Position = o_ClipSpace;
	o_TexCoords = vec2(in_Pos.x / 2.0 + 0.5, in_Pos.y/2.0 + 0.5) * tiling;
}