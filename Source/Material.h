#pragma once

class Shader;

class Material
{
public:
	virtual void ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection) = 0;

	Shader* shader;
};