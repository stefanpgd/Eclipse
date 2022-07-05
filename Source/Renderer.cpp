#include "Precomp.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "DiffuseMaterial.h"
#include "DefaultMaterial.h"
#include "SceneLoader.h"
#include <iomanip>
#include <ctime>
#include <sstream>

inline float Random01()
{
	return float(rand() / (RAND_MAX + 1.0));
}

inline float RandomInRange(float min, float max)
{
	return min + (max - min) * Random01();
}

bool useWindow = false;

void Renderer::Run()
{
	if (!isRunning)
	{
		isRunning = true;
		Initialize();
		Update();
		CloseRenderer();
	}
}

void Renderer::Initialize()
{
	if (!isInitialized)
	{
		isInitialized = true;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Eclipse", NULL, NULL);

		if (window == NULL)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window, MouseCallback);
		glfwSetScrollCallback(window, MouseScrollCallback);
		glfwSetKeyCallback(window, KeyboardCallback);

		if (FocusWindow)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		// GLAD: Load all OpenGL Function pointers based on device/platform
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glEnable(GL_DEPTH_TEST);

		camera = Camera::GetInstance();
		camera->Initialize(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));

		imgui = ImguiHandler::GetInstance();
		imgui->Initialize(window);

		SceneLoader sceneLoader;

		if (!sceneLoader.LoadScene("testScene", objects))
		{
			CreateTestScene();
		}
	}
}

void Renderer::Update()
{
	ImguiHandler* imgui = ImguiHandler::GetInstance();

	bool showSceneDetails = false;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		DrawCalls = 0;

		glViewport(0, 0, ScreenWidth, ScreenHeight);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timeElasped += deltaTime;
		GlobalLightDirection.x = 0.3f;
		GlobalLightDirection.y = -0.75f;
		GlobalLightDirection.z = cosf(timeElasped) * 0.3f;
		//normalize(GlobalLightDirection);

		imgui->Update();

		ProcessContinuesInputEvents();

		if (FocusWindow)
		{
			camera->Update(deltaTime);
		}
		else
		{
			camera->FirstMouse = true;
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}

		editor.DrawEditor(objects, consoleLog, deltaTime);

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->Duplicate)
			{
				Object* obj = new Object(*objects[i]);
				objects.push_back(obj);
				objects[i]->Duplicate = false;
			}
		}

		imgui->Draw();

		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end();)
		{
			if((*it)->Deleted)
			{
				Object* obj = (*it);
				delete obj;
				it = objects.erase(it);
			}
			else
			{
				++it;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Renderer::CloseRenderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	SceneLoader loader;
	loader.SaveScene("testScene", objects);
}

void Renderer::CreateTestScene()
{
	DiffuseMaterial* diffuseMat = new DiffuseMaterial();
	Object* sponza = new Object("Sponza/sponza.obj", diffuseMat);
	objects.push_back(sponza);
	sponza->materialIndex = 0;
	sponza->name = "Sponza";

	DefaultMaterial* defaultMat = new DefaultMaterial();
	Object* Lucy = new Object("Lucy.obj", defaultMat);
	objects.push_back(Lucy);
	Lucy->materialIndex = 1;
	Lucy->name = "Lucy";
}

void Renderer::ProcessContinuesInputEvents()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraDirection::Front);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraDirection::Back);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraDirection::Left);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraDirection::Right);
	}
}

void Renderer::ProcessSingleInputEvents(int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		FocusWindow = false;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		FocusWindow = true;
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		Camera::GetInstance()->Sprinting = true;
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		Camera::GetInstance()->Sprinting = false;
	}
}

void Renderer::ConsoleLog(std::string message, WarningLevel warningLevel, bool showTimeStamp)
{
	std::string consoleMessage;
	switch (warningLevel)
	{
	case WarningLevel::Log:
		consoleMessage += "[Log] ";
		break;

	case WarningLevel::Warning:
		consoleMessage += "[Warning] ";
		break;

	case WarningLevel::Error:
		consoleMessage += "[ERROR] ";
		break;
	}

	if (showTimeStamp)
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%H:%M:%S");
		std::string time = "(" + oss.str() + ") ";
		consoleMessage += time;
	}

	consoleMessage += message.data();
	consoleLog.push_back(consoleMessage);
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer::GetInstance()->ProcessSingleInputEvents(key, action);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	Camera::GetInstance()->ProcessMouseMovement(xPos, yPos);
}

void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos)
{
	Camera::GetInstance()->ProcessMouseScroll(static_cast<float>(yPos));
}