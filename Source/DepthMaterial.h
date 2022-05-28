#pragma once

#include "Material.h"

class Camera;

class DepthMaterial : public Material
{
public:
	DepthMaterial();

	virtual void ActivateMaterial(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
private:
	Camera* camera;
};