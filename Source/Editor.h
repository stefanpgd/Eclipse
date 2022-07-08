#pragma once

#include <vector>
#include <glm/glm.hpp>

class Object;

class Editor
{
public:
	Editor();
	void DrawEditor(std::vector<Object*>& objects, std::vector<std::string>& consoleLog, float deltaTime);
	void LoadEditorSaveProfile(std::vector<bool> settings);

private:
	void SetWindowParameters();
	void DrawMenubar();
	void DrawSceneWindow(std::vector<Object*>& objects);
	void DrawObjectDetails(Object* object);
	void DrawGizmos(Object* object);
	void DrawConsole(std::vector<std::string>& consoleLog);
	void DrawStatistics();

	void GetAllModelFilePaths(std::vector<std::string>& files, std::string path, std::string originalPath);
	void DrawVector3Edit(const std::string& name, glm::vec3& data, float resetValue);

public:

	// Menu bar toggles
	bool showSceneObjects = true;
	bool showObjectCreation = false;
	bool showCameraSettings = false;
	bool showObjectDetails = true;
	bool showStatistics = false;
	bool showGizmos = true;
	bool showConsole = false;

private:
	float lastDeltaTime = 1.0f;

	// FPS Counter (Top-Right)
	float frameCounterTimer = 0.0f;
	float frameCounterInterval = 0.075f;
	int FPS = 0;

	// Scene details
	int selectedObject = 0;

	// Object Creation
	std::string modelFolderLoadPath = "Assets/Models";
	std::vector<std::string> modelFilePaths;
	std::string newObjectName;
	int activeModelIndex = 0;
	int modelMaterialIndex = 0;
	float modelUniformScale = 1.0f;
	Object* objectToAdd = nullptr;

	// Object Details
	std::string placeholderName = "";

	// Console
	bool autoScrollConsole = true;
	bool showLogs = true;
	bool showWarnings = true;
	bool showErrors = true;
};