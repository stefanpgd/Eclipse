#pragma once

struct GLFWwindow;
class Camera;
class Object;
class ImguiHandler;

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
	glm::vec3 GlobalLightDirection;
	glm::vec3 GlobalLightColor = glm::vec3(1.0, 1.0, 1.0);

private:
	void Initialize();
	void Update();
	void CloseRenderer();
	void CreateTestScene();

private:
	bool isRunning = false;
	bool isInitialized = false;

	Editor editor;
	Camera* camera;
	GLFWwindow* window;
	ImguiHandler* imgui;

	std::vector<Object*> objects;
	std::vector<std::string> consoleLog;

	bool FocusWindow = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float timeElasped = 0.0f;

	float lightSpeed = 0.1f;
	bool disco = false;
};