#version 330 core
out vec4 FragColor;
in vec3 FragPosition;

void main()
{
	vec3 start = vec3(1.0, 1.0, 1.0);

	FragColor = vec4(start, 1.0);
}