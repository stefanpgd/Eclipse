#pragma once

#include "Material.h"

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial();

	virtual void ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
};