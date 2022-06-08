#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TextureCoord;
in vec3 Normal;

uniform bool useTexture;
uniform sampler2D diffuseTexture;

void main()
{
	if(useTexture)
	{
		FragColor = texture(diffuseTexture, TextureCoord);
	}
	else
	{
		vec3 normalColor = (normalize(Normal) * 0.5) + 0.5;
		FragColor = vec4(normalColor, 1.0);
	}
}