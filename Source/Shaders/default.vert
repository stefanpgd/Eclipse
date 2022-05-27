#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec3 FragPosition;
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    FragPosition = vec3(gl_Position);
    Normal = mat3(transpose(inverse(model))) * aNormal;
}