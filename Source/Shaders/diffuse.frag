#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TextureCoord;

uniform sampler2D texture_diffuse1;

void main()
{
	vec4 color = texture(texture_diffuse1, TextureCoord);

	if(color.a < 0.5)
	{
		discard;
	}

	FragColor = color;
}