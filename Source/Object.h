#pragma once

#include "Transform.h"
#include "MeshRenderer.h"

class Material;
class Camera;
class Light;

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
	virtual void Draw(std::vector<Light*>& lights);
	virtual void DrawShadowMap(Shader* depthShader);

public:
	Transform transform;
	MeshRenderer meshRenderer;
	Material* material;

	std::string name{ "Object" };
	std::string modelFileName;
	int materialIndex;

	bool isActive = true;
	bool Deleted = false;
	bool Duplicate = false;

	int ID;

private:
	Camera* camera;
};