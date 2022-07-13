#pragma once

// Credits: Joey de Vries
// https://learnopengl.com/Model-Loading/Assimp

class Shader;

struct AVertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec3 Tangent;
};

struct ATexture
{
	unsigned int ID;
	std::string Type;
	std::string Path;
};

class Mesh
{
public:
	Mesh(std::vector<AVertex> vertices, std::vector<unsigned int> indices, std::vector<ATexture> textures, 
		vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float shininess);
	void Draw(Shader* shader);

private:
	void SetupMesh();

public:
	std::vector<AVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<ATexture> textures;

	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	float Shininess;

private:
	unsigned int VBO, VAO, EBO;
};