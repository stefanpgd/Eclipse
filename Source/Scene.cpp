#include "precomp.h"
#include "Object.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"
#include "Skybox.h"

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
	LoadScene();
	skybox = new Skybox("Assets/Skyboxes/Space");
}

void Scene::Update(float deltaTime)
{
	DuplicationAndDeletionOfEntities();

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

	skybox->Draw();
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

void Scene::DuplicationAndDeletionOfEntities()
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

	// Duplication Function
	for (int i = 0; i < Lights.size(); i++)
	{
		if (Lights[i]->Duplicate)
		{
			Light* light = new Light(*Lights[i]);
			Lights.push_back(light);
			Lights[i]->Duplicate = false;
		}
	}

	// Delete Function
	for (std::vector<Light*>::iterator it = Lights.begin(); it != Lights.end();)
	{
		if ((*it)->Deleted)
		{
			Light* light = (*it);
			delete light;
			it = Lights.erase(it);
		}
		else
		{
			++it;
		}
	}
}