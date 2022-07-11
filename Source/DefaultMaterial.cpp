#include "precomp.h"
#include "Shader.h"
#include "TextureLibrary.h"
#include "Texture.h"
#include "DefaultMaterial.h"
#include "Camera.h"

DefaultMaterial::DefaultMaterial()
{
	shader = new Shader("ReflectSkybox.vert", "ReflectSkybox.frag");
}

DefaultMaterial::DefaultMaterial(std::string textureName)
{
	shader = new Shader("ReflectSkybox.vert", "ReflectSkybox.frag");
	texture = TextureLibrary::GetInstance()->GetTexture(textureName);
}

void DefaultMaterial::ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->Use();
	
	mat4 MVP = projection * view * model;
	shader->SetMat4("MVP", MVP);
	shader->SetMat4("model", model);
	shader->SetVec3("cameraPosition", Camera::GetInstance()->CameraPosition);

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