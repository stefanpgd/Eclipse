#version 330 core
out vec4 FragColor;
in vec3 FragPosition;
in vec3 Normal;

void main()
{
	vec3 start = vec3(1.0, 1.0, 1.0);
	float c = 1 - FragPosition.z * 0.05;
	vec3 normalColor = (Normal * 0.5) + 0.5;
	FragColor = vec4(normalColor * c, 1.0);
}