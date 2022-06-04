#pragma once

class Object;

class SceneLoader
{
public:
	SceneLoader();

	void SaveScene(std::string sceneName, std::vector<Object*>& objects);
	bool LoadScene(std::string sceneName, std::vector<Object*>& objects);
};