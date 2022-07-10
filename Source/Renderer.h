#pragma once

struct GLFWwindow;
class Camera;
class Object;
class ImguiHandler;
class Scene;

enum class WarningLevel
{
	Log,
	Warning,
	Error
};

#include <vector>
#include <string>
#include "Editor.h"

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos);

class Renderer
{
public:
	static Renderer* GetInstance()
	{
		static Renderer instance;
		return &instance;
	}

	void Run();
	void ProcessContinuesInputEvents();
	void ProcessSingleInputEvents(int key, int action);
	void ConsoleLog(std::string message, WarningLevel warningLevel = WarningLevel::Log, bool showTimeStamp = true);

public:
	int DrawCalls = 0;

private:
	void Initialize();
	void Update();
	void CloseRenderer();

private:
	bool isRunning = false;
	bool isInitialized = false;

	Editor* editor;
	Scene* activeScene;
	Camera* camera;
	GLFWwindow* window;
	ImguiHandler* imgui;

	std::vector<std::string> consoleLog;

	bool FocusWindow = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float timeElasped = 0.0f;
};