#pragma once

#include "Light.h"
#include "SceneLoader.h"

class Object;
class Skybox;
class Shader;

class Scene
{
public:
	Scene(std::string sceneName);

	void Update(float deltaTime);
	void Draw();
	void DrawShadowMap();
	void SaveScene();

private:
	void LoadScene();
	void DuplicationAndDeletionOfEntities();

public:
	std::vector<Object*> Objects;
	std::vector<Light*> Lights;

private:
	Skybox* skybox;
	Shader* depthMapShader;

	SceneLoader sceneLoader;
	std::string sceneName;
};