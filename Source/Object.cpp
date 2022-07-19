#include "precomp.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "DefaultMaterial.h"
#include "DepthMaterial.h"
#include "DiffuseMaterial.h"
#include "Object.h"
#include "Light.h"
#include "Shader.h"

Object::Object()
{
	ID = rand();
	material = new DepthMaterial();
	meshRenderer.Initialize();
	camera = Camera::GetInstance();
}

Object::~Object()
{
	camera = nullptr;
	material = nullptr;
}

Object::Object(const Object& obj)
{
	ID = rand();
	name = obj.name;
	material = obj.material;
	materialIndex = obj.materialIndex;
	modelFileName = obj.modelFileName;
	meshRenderer.Initialize(modelFileName);
	camera = obj.camera;
	transform = obj.transform;
}

Object::Object(std::string fileName)
{
	ID = rand();
	material = new DepthMaterial();
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
	modelFileName = fileName;
}

Object::Object(std::string fileName, Material* material)
{
	ID = rand();
	this->material = material;
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
	modelFileName = fileName;
}

Object::Object(ObjectSaveData& data)
{
	ID = rand();
	name = data.name;
	modelFileName = data.modelFileName;

	transform.SetTransformDataFromArray(data.transformInfo);

	if (data.modelFileName != "")
	{
		meshRenderer.Initialize(data.modelFileName);
	}
	else
	{
		meshRenderer.Initialize();
	}

	materialIndex = data.materialIndex;

	if (data.materialIndex == 0)
	{
		material = new DiffuseMaterial();
	}
	else
	{
		material = new DefaultMaterial();
	}

	camera = Camera::GetInstance();
}

void Object::Update(float deltaTime)
{
}

void Object::Draw(std::vector<Light*>& lights)
{
	if (isActive)
	{
		material->ActivateMaterial(transform.GetModelMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		material->shader->SetMat4("lightSpaceMatrix", lights[0]->GetLightSpaceMatrix());
		material->shader->SetInt("shadowMap", 10);

		if (lights.size() > 1)
		{
			material->shader->SetInt("amountOfPointLights", lights.size() - 1);
		}
		else
		{
			material->shader->SetInt("amountOfPointLights", 0);
		}

		for (int i = 0; i < lights.size(); i++)
		{
			lights[i]->BindLightData(material->shader, i);
		}

		meshRenderer.Draw(material->shader);
	}
}

void Object::DrawShadowMap(Shader* depthShader)
{
	if (isActive)
	{
		depthShader->SetMat4("model", transform.GetModelMatrix());
		meshRenderer.Draw(material->shader);
	}
}