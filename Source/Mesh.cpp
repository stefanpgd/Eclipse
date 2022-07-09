#include "precomp.h"
#include "Shader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Camera.h"

Mesh::Mesh(std::vector<AVertex> vertices, std::vector<unsigned int> indices, std::vector<ATexture> textures, 
	vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float shininess) : 
	Vertices(vertices), Indices(indices), textures(textures), AmbientColor(ambientColor), DiffuseColor(diffuseColor), SpecularColor(specularColor), Shininess(shininess)
{
	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(AVertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Normal));

	// Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	shader->SetVec3("cameraPosition", Camera::GetInstance()->CameraPosition);
	shader->SetVec3("material.Ambient", AmbientColor);
	shader->SetVec3("material.Diffuse", DiffuseColor);
	shader->SetVec3("material.Specular", SpecularColor);
	shader->SetFloat("material.Shininess", Shininess);

	int specularTextureAmount = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].Type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
			specularTextureAmount++;
		}

		shader->SetInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	if (specularTextureAmount == 0)
	{
		shader->SetInt("texture_specular1", 0);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	Renderer::GetInstance()->DrawCalls++;
}