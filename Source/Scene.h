#pragma once

#include "Light.h"
#include "SceneLoader.h"

class Object;

class Scene
{
public:
	Scene(std::string sceneName);

	void Update(float deltaTime);
	void Draw();
	void SaveScene();

private:
	void LoadScene();

public:
	std::vector<Object*> Objects;
	std::vector<Light*> Lights;

private:
	SceneLoader sceneLoader;
	std::string sceneName;
};