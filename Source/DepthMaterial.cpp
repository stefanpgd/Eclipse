#include "precomp.h"
#include "Camera.h"
#include "Shader.h"
#include "DepthMaterial.h"

DepthMaterial::DepthMaterial()
{
	shader = new Shader("default.vert", "depthTest.frag");
	camera = Camera::GetInstance();
}

void DepthMaterial::ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->Use();

	mat4 MVP = projection * view * model;
	shader->SetMat4("MVP", MVP);
	shader->SetMat4("model", model);
	shader->SetFloat("near", camera->Near);
	shader->SetFloat("far", camera->Far);
}