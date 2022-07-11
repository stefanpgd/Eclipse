#pragma once

#include <vector>
#include <glm/glm.hpp>

class Object;
class Light;

class Editor
{
public:
	static Editor* GetInstance()
	{
		static Editor instance;
		return &instance;
	}

	Editor();

	void DrawEditor(std::vector<Object*>& objects, std::vector<Light*>& lights, std::vector<std::string>& consoleLog, float deltaTime);
	void LoadEditorSaveProfile(std::vector<bool> settings);

private:
	void SetWindowParameters();
	void DrawMenubar();
	void DrawSceneWindow(std::vector<Object*>& objects, std::vector<Light*>& lights);
	void DrawGizmos(Object* object);
	void DrawGizmos(Light* light);
	void DrawConsole(std::vector<std::string>& consoleLog);
	void DrawStatistics();

	void DrawSelectedObjectDetails(Object* object);
	void DrawSelectedLightDetails(std::vector<Light*>& lights, Light* light);

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
	bool showSceneLights = true;

private:
	float lastDeltaTime = 1.0f;

	// FPS Counter (Top-Right)
	float frameCounterTimer = 0.0f;
	float frameCounterInterval = 0.075f;
	int FPS = 0;

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
	int selectedObject = 0;
	int selectedLight = -1;
	bool isSelectedAnObject = true;

	// Console
	bool autoScrollConsole = true;
	bool showLogs = true;
	bool showWarnings = true;
	bool showErrors = true;
};