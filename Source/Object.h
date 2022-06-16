#pragma once

#include "Transform.h"
#include "MeshRenderer.h"

class Material;
class Camera;

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

public:
	Transform transform;
	MeshRenderer meshRenderer;

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
};