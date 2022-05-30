#include "precomp.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "DefaultMaterial.h"
#include "DepthMaterial.h"
#include "Object.h"

Object::Object()
{
	material = new DepthMaterial();
	meshRenderer.Initialize();
	camera = Camera::GetInstance();
}

Object::Object(std::string fileName)
{
	material = new DepthMaterial();
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
}

Object::Object(std::string fileName, Material* material)
{
	this->material = material;
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
}

void Object::Update(float deltaTime)
{
	transform.Rotation.x += 25.0f * deltaTime;
	transform.Rotation.y += 15.0f * deltaTime;
	transform.Rotation.z += 5.0f * deltaTime;
}

void Object::Draw()
{
	if (isActive)
	{
		material->ActivateMaterial(transform.GetModelMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		meshRenderer.Draw(material->shader);
	}
}