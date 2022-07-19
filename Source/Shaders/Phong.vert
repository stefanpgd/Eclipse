#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPosition;
out vec2 TextureCoord;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    FragPosition = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TextureCoord = aTexCoord;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPosition, 1.0);
}