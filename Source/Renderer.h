#pragma once

struct GLFWwindow;

class Renderer
{
public:
	void Run();

private:
	void Initialize();
	void Update();
	void CloseRenderer();

	void ProcessInput(GLFWwindow* window);

private:
	GLFWwindow* window;

	const int ScreenWidth = 800;
	const int ScreenHeight = 600;
};