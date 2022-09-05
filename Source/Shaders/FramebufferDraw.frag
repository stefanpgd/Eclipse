#version 330 core
out vec4 FragColor;

in vec2 TextureCoord;

uniform sampler2D screenTexture;
uniform vec2 rOffset;
uniform vec2 gOffset;
uniform vec2 bOffset;
uniform float exposure;
uniform float vignette;

void main()
{
//	float r = texture(screenTexture, TextureCoord - rOffset).r;
//	float g = texture(screenTexture, TextureCoord - gOffset).g;
//	float b = texture(screenTexture, TextureCoord - bOffset).b;
//	vec3 ChromaticColor = vec3(r,g,b);
	
	const float gamma = 2.2;
	vec3 color = texture(screenTexture, TextureCoord).rgb;
	vec3 mapped = vec3(1.0) - exp(-color * exposure);
	//mapped = pow(mapped, vec3(1.0 / gamma));
	FragColor = vec4(mapped,1.0);

	//FragColor = texture(screenTexture, TextureCoord);
}