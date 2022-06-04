#pragma once

struct GLFWwindow;
class Camera;
class Object;
class ImguiHandler;
#include <vector>

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

	bool FocusWindow = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
};