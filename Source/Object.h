#pragma once

#include "Transform.h"
#include "MeshRenderer.h"

class Material;

struct ObjectSaveData
{
	std::string name;
	std::string modelFileName;
	float transformInfo[9];
	int materialIndex;
};

class Object
{
public:
	Object();
	Object(std::string fileName);
	Object(std::string fileName, Material* material);
	Object(ObjectSaveData& data);

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void EditorInfo();

public:
	Transform transform;

	std::string name{ "Object" };
	std::string modelFileName;
	int materialIndex;

	bool isActive = true;

private:
	int ID;

	Camera* camera;
	Material* material;
	MeshRenderer meshRenderer;
};