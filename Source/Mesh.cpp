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

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Tangent));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{
	shader->SetVec3("cameraPosition", Camera::GetInstance()->CameraPosition);
	shader->SetVec3("material.Ambient", AmbientColor);
	shader->SetVec3("material.Diffuse", DiffuseColor);
	shader->SetVec3("material.Specular", SpecularColor);
	shader->SetFloat("material.Shininess", Shininess);

	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i].Type == "texture_diffuse")
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			shader->SetInt("texture_diffuse", 0);
		}

		if (textures[i].Type == "texture_specular")
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			shader->SetInt("texture_specular", 1);
		}

		if (textures[i].Type == "texture_normal")
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			shader->SetInt("texture_normal", 2);
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	Renderer::GetInstance()->DrawCalls++;
}