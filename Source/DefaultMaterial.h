#pragma once

#include "Material.h"

class Texture;

class DefaultMaterial : public Material
{
public:
	DefaultMaterial();
	DefaultMaterial(std::string textureName);

	virtual void ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
	Texture* texture = nullptr;
};