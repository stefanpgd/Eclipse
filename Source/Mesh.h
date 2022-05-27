#pragma once

// Credits: Joey de Vries
// https://learnopengl.com/Model-Loading/Assimp

struct AVertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(std::vector<AVertex> vertices, std::vector<unsigned int> indices);
	void Draw();

private:
	void SetupMesh();

public:
	std::vector<AVertex> Vertices;
	std::vector<unsigned int> Indices;

private:
	unsigned int VBO, VAO, EBO;
};