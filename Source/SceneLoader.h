#pragma once

class Object;

class SceneLoader
{
public:
	SceneLoader();

	void SaveScene(std::string sceneName, std::vector<Object*>& objects);
	void LoadScene(Object* temp);
};