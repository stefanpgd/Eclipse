#pragma once

#include <vector>
class Object;

class Editor
{
public:
	Editor();
	void DrawEditor(std::vector<Object*>& objects, float deltaTime);

private:
	void SetWindowParameters();
	void DrawMenubar();
	void DrawSceneWindow(std::vector<Object*>& objects);
	void DrawObjectDetails(Object* object);

	void GetAllModelFilePaths(std::vector<std::string>& files, std::string path, std::string originalPath);

private:
	float lastDeltaTime = 1.0f;

	// FPS Counter (Top-Right)
	float frameCounterTimer = 0.0f;
	float frameCounterInterval = 0.075f;
	int FPS = 0;

	// Menu bar toggles
	bool showSceneObjects = true;
	bool showObjectCreation = false;
	bool showCameraSettings = false;
	bool showObjectDetails = true;
	bool showStatistics = false;

	// Scene details
	int selectedObject = 0;

	// Object Creation
	std::string modelFolderLoadPath = "Assets/Models";
	std::vector<std::string> modelFilePaths;
	std::string newObjectName;
	int activeModelIndex = 0;
	int modelMaterialIndex = 0;
	float modelUniformScale = 1.0f;
};