#pragma once

class Object;
class Editor;

class SceneLoader
{
public:
	SceneLoader();

	void SaveScene(std::string sceneName, std::vector<Object*>& objects, Editor* editor);
	bool LoadScene(std::string sceneName, std::vector<Object*>& objects, Editor* editor);
};