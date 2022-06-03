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

	// Window Interaction
	void DrawText(std::string text);
	void DrawText(std::string windowName, std::string text);

	void SliderFloat(std::string name, float& value, float min, float max);
	void SliderFloat(std::string windowName, std::string name, float& value, float min, float max);
	void SliderVector3(std::string name, glm::vec3& value, float min, float max);
	void SliderVector3(std::string windowName, std::string name, glm::vec3& value, float min, float max);

	void InputVector3(std::string name, glm::vec3& value);
	void InputVector3(std::string windowName, std::string name, glm::vec3& value);


private:
	GLFWwindow* window;

	bool isInitialized = false;
	bool windowActivated = false;
};