#include "precomp.h"
#include "Model.h"
#include "ModelLibrary.h"
#include "Shader.h"
#include "MeshRenderer.h"

void MeshRenderer::Initialize()
{
	glGenVertexArrays(1, &VBO);
	glGenBuffers(1, &VAO);

	// Bind object and push buffer object to GPU
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// Setup attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);					// Vertex Pos
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Normals
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexCount = 36;
	meshSetup = true;
}

void MeshRenderer::Initialize(std::string fileName)
{
	std::string path = "Assets/Models/" + fileName;

	model = ModelLibrary::GetInstance()->GetModel(path);

	meshSetup = true;
	usesModel = true;
}

void MeshRenderer::Draw(Shader* shader)
{
	if (!meshSetup)
	{
		throw std::runtime_error("ERROR: Mesh has not yet been initialized, so draw calls can't be made!");
	}

	if (usesModel)
	{
		model->Draw(shader);
	}
	else
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}