#version 330 core
out vec4 FragColor;
in vec3 FragPosition;

void main()
{
	vec3 start = vec3(1.0, 1.0, 1.0);
	float c = 1 - FragPosition.z * 0.1f;

	FragColor = vec4(c,c,c, 1.0);
}