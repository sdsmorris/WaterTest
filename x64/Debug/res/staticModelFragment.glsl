#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

//FOG
in float visibility;
uniform vec3 skyColor;

uniform sampler2D texture_diffuse1;

uniform bool useFog = false;

void main()
{    
    fragColor = texture(texture_diffuse1, TexCoords);
    
    //FOG
    if (useFog)
        fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}