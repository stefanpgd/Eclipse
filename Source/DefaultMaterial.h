#pragma once

#include "Material.h"

class DefaultMaterial : public Material
{
public:
	DefaultMaterial();

	virtual void ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
};