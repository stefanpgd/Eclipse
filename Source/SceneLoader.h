#pragma once

class Object;
class Light;
class Editor;

class SceneLoader
{
public:
	void SaveScene(std::string sceneName, std::vector<Object*>& objects, std::vector<Light*>& lights);
	bool LoadScene(std::string sceneName, std::vector<Object*>& objects, std::vector<Light*>& lights);
};