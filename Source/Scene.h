#pragma once

#include "Light.h"
#include "SceneLoader.h"

class Object;
class Skybox;

class Scene
{
public:
	Scene(std::string sceneName);

	void Update(float deltaTime);
	void Draw();
	void SaveScene();

private:
	void LoadScene();
	void DuplicationAndDeletionOfEntities();

public:
	std::vector<Object*> Objects;
	std::vector<Light*> Lights;

private:
	Skybox* skybox;

	SceneLoader sceneLoader;
	std::string sceneName;
};