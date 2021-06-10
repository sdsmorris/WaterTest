#version 330 core

out vec4 out_FragColor;

in vec2 o_TexCoords;

uniform sampler2D s_Tex;

void main()
{
	out_FragColor = texture(s_Tex, o_TexCoords);
	//out_FragColor = vec4(o_TexCoords.x, o_TexCoords.y, 0, 1.0);
}