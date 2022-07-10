#include "precomp.h"
#include "Object.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
	LoadScene();
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
	sceneLoader.SaveScene(sceneName, Objects, Lights);
}

void Scene::LoadScene()
{
	if (!sceneLoader.LoadScene(sceneName, Objects, Lights))
	{
		Renderer::GetInstance()->ConsoleLog("Couldn't find a scene with the name: " + sceneName, WarningLevel::Warning);
	}
}