#pragma once

// Credits: Joey de Vries
// https://learnopengl.com/Model-Loading/Assimp

class Shader;

struct AVertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
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
	Mesh(std::vector<AVertex> vertices, std::vector<unsigned int> indices, std::vector<ATexture> textures);
	void Draw(Shader* shader);

private:
	void SetupMesh();

public:
	std::vector<AVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<ATexture> textures;

private:
	unsigned int VBO, VAO, EBO;
};