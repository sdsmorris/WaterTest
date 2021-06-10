#version 330 core

out vec4 fragColor;

in vec2 o_TexCoords;
in vec4 o_ClipSpace;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;

uniform float moveFactor;
const float waveStrength = 0.05;

void main()
{
	vec2 ndc = (o_ClipSpace.xy/o_ClipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = (texture(dudvMap, vec2(o_TexCoords.x + moveFactor, o_TexCoords.y)).rb * 2 - 1) * waveStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(-o_TexCoords.x + moveFactor, o_TexCoords.y + moveFactor)).rb * 2 - 1) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);
	fragColor = mix(reflectColor, refractColor, 0.5);
	//fragColor = vec4(0, 0, 1, 1);
	
}