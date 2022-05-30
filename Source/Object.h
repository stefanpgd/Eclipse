#pragma once

#include "Transform.h"

class Material;
class MeshRenderer;

class Object
{
public:
	Object();
	Object(std::string fileName);
	Object(std::string fileName, Material* material);

	virtual void Update(float deltaTime);
	virtual void Draw();

public:
	Transform transform;
	
	std::string name{ "Object" };
	bool isActive = true;

private:
	Camera* camera;
	Material* material;
	MeshRenderer meshRenderer;
};