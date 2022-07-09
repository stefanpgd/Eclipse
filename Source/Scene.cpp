#include "precomp.h"
#include "Object.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
	LoadScene();

	// Temp
	Light* directionalLight = new Light();
	directionalLight->IsPointLight = false;
	directionalLight->Color = vec3(0.8f);
	directionalLight->GlobalLightRotation = normalize(vec3(0.2f, -1.0f, 0.3f));
	Lights.push_back(directionalLight);

	for (int i = 0; i < 15; i++)
	{
		Light* pointLight = new Light();
		pointLight->Color = vec3(RandomInRange(0.0f, 1.0f), RandomInRange(0.0f, 1.0f), RandomInRange(0.0f, 1.0f));
		pointLight->Intensity = RandomInRange(10.0f, 100.0f);
		pointLight->Position = vec3(RandomInRange(-800.0f, 800.0f), RandomInRange(5.0f, 120.0f), RandomInRange(-200.0f, 200.0f));
		pointLight->basePosition = pointLight->Position;
		pointLight->timeOffset = RandomInRange(0.0f, 100.0f);
		Lights.push_back(pointLight);
	}
}

void Scene::Update(float deltaTime)
{
	// Duplication Function
	for (int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i]->Duplicate)
		{
			Object* obj = new Object(*Objects[i]);
			Objects.push_back(obj);
			Objects[i]->Duplicate = false;
		}
	}

	// Temp
	for (int i = 0; i < Lights.size(); i++)
	{
		if (Lights[i]->IsPointLight)
		{
			timeElasped += deltaTime * 0.25f;

			vec3 pos = Lights[i]->basePosition;
			pos.x += cosf(timeElasped + Lights[i]->timeOffset) * 100.0f;
			pos.y += sinf(timeElasped + Lights[i]->timeOffset) * 25.0f;
			pos.z += sinf(timeElasped + Lights[i]->timeOffset) * 75.0f;
			Lights[i]->Position = pos;
		}
	}

	// Delete Function
	for (std::vector<Object*>::iterator it = Objects.begin(); it != Objects.end();)
	{
		if ((*it)->Deleted)
		{
			Object* obj = (*it);
			delete obj;
			it = Objects.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->Update(deltaTime);
	}
}

void Scene::Draw()
{
	for (int i = 0; i < Objects.size(); i++)
	{
		// For now, light's are passed to individual objects, but after the rework they should be passed to a material/shader technique instead
		// so they only have to be bound once.
		Objects[i]->Draw(Lights);
	}
}

void Scene::SaveScene()
{
	sceneLoader.SaveScene(sceneName, Objects);
}

void Scene::LoadScene()
{
	if (!sceneLoader.LoadScene(sceneName, Objects))
	{
		Renderer::GetInstance()->ConsoleLog("Couldn't find a scene with the name: " + sceneName, WarningLevel::Warning);
	}
}