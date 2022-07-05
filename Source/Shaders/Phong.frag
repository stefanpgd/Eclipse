#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TextureCoord;

uniform sampler2D texture_diffuse1;
uniform vec3 materialColor;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightAmbient = vec3(0.75, 0.75, 0.75);

	vec3 ambient = lightColor * lightAmbient * vec3(materialColor);
	FragColor = vec4(ambient, 1.0);
}