#include "precomp.h"
#include "Object.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"
#include "Skybox.h"
#include "Shader.h"

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
	LoadScene();
	skybox = new Skybox("Assets/Skyboxes/Space");
	depthMapShader = new Shader("depthMask.vert", "depthMask.frag");
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
	ImGui::Begin("Test Shadows");
	ImGui::DragFloat("Near", &Lights[0]->n, 10.0f, 0.1f, 100.0f);
	ImGui::DragFloat("Far", &Lights[0]->f, 10.0f, 0.01f, 100000.0f);
	ImGui::DragFloat("height", &Lights[0]->height, 10.0f, 100.0f, 10000.0f);
	ImGui::DragFloat("Ortho Size", &Lights[0]->orthoSize, 10.0f, 0.01f, 10000.0f);
	ImGui::DragFloat3("testPosition", &Lights[0]->testPosition[0], 1.0f, -5000.0f, 5000.0f);
	ImGui::Checkbox("Use Directional", &Lights[0]->tryOutDirectionalMethod);
	ImGui::End();

	for (int i = 0; i < Objects.size(); i++)
	{
		// For now, light's are passed to individual objects, but after the rework they should be passed to a material/shader technique instead
		// so they only have to be bound once.
		Objects[i]->Draw(Lights);
	}

	skybox->Draw();
}

void Scene::DrawShadowMap()
{
	depthMapShader->Use();
	depthMapShader->SetMat4("lightSpaceMatrix", Lights[0]->GetLightSpaceMatrix());

	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->DrawShadowMap(depthMapShader);
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