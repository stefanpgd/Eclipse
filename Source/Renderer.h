#pragma once

struct GLFWwindow;
class Camera;
class Object;
class ImguiHandler;
#include <vector>
#include <string>

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

private:
	void Initialize();
	void Update();
	void CloseRenderer();
	void CreateTestScene();

private:
	bool isRunning = false;
	bool isInitialized = false;

	Camera* camera;
	GLFWwindow* window;
	ImguiHandler* imgui;

	std::vector<Object*> objects;
	std::vector<std::string> modelFilePaths;
	int activeModelIndex = 0;

	// temp
	int selectedModel = 0;
	int selectedMaterial = 0;
	std::string newObjectName;
	float uniformScale = 1.0f;

	bool FocusWindow = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
};