#pragma once

class ImguiHandler
{
public:
	static ImguiHandler* GetInstance()
	{
		static ImguiHandler instance;
		return &instance;
	}

	void Initialize(GLFWwindow* window);
	void Update();
	void Draw();

private:
	void SetupStyle();

private:
	GLFWwindow* window;
	bool isInitialized = false;
};