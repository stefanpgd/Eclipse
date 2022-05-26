#include "precomp.h"
#include "Shader.h"
#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial()
{
	shader = new Shader("default.vert", "default.frag");
}

void DefaultMaterial::ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->Use();
	mat4 MVP = projection * view * model;
	shader->SetMat4("MVP", MVP);
}