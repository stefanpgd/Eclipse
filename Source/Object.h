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
	~Object();
	Object(const Object& obj);
	Object(std::string fileName);
	Object(std::string fileName, Material* material);
	Object(ObjectSaveData& data);

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void EditorInfo(bool show);

public:
	Transform transform;

	std::string name{ "Object" };
	std::string modelFileName;
	int materialIndex;

	bool isActive = true;
	bool Deleted = false;
	bool Duplicate = false;

private:
	int ID;

	Camera* camera;
	Material* material;
	MeshRenderer meshRenderer;
};