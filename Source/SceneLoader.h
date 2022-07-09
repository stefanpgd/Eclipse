#pragma once

class Object;
class Editor;

class SceneLoader
{
public:
	void SaveScene(std::string sceneName, std::vector<Object*>& objects);
	bool LoadScene(std::string sceneName, std::vector<Object*>& objects);
};