#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPosition;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
    FragPosition = vec3(gl_Position);
}