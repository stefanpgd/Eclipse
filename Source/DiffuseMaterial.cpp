#include "precomp.h"
#include "Shader.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial()
{
	shader = new Shader("default.vert", "diffuse.frag");
}

void DiffuseMaterial::ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->Use();

	mat4 MVP = projection * view * model;
	shader->SetMat4("MVP", MVP);
	shader->SetMat4("model", model);
}