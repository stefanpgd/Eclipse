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

	// Window creation & Activation
	void CreateWindow(std::string title);
	void CreateWindow(glm::vec2 position, glm::vec2 size, std::string title);
	void ActivateWindow(std::string windowName);
	void DisableWindow();

private:
	void SetupStyle();

private:
	GLFWwindow* window;

	bool isInitialized = false;
	bool windowActivated = false;
};