#pragma once

#include "Transform.h"

class Material;
class MeshRenderer;

class Object
{
public:
	Object();

	virtual void Update();
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