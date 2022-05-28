#include "precomp.h"
#include "Shader.h"
#include "TextureLibrary.h"
#include "Texture.h"
#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial()
{
	shader = new Shader("default.vert", "default.frag");
}

DefaultMaterial::DefaultMaterial(std::string textureName)
{
	shader = new Shader("default.vert", "default.frag");
	texture = TextureLibrary::GetInstance()->GetTexture(textureName);
}

void DefaultMaterial::ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->Use();
	
	mat4 MVP = projection * view * model;
	shader->SetMat4("MVP", MVP);
	shader->SetMat4("model", model);

	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->ID);

		shader->SetBool("useTexture", true);
		shader->SetInt("diffuseTexture", 0);
	}
	else
	{
		shader->SetBool("useTexture", false);
	}
}