#include "precomp.h"
#include "Renderer.h"

#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "DiffuseMaterial.h"
#include "DefaultMaterial.h"
#include "SceneLoader.h"

inline float Random01()
{
	return float(rand() / (RAND_MAX + 1.0));
}

inline float RandomInRange(float min, float max)
{
	return min + (max - min) * Random01();
}

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
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		imgui->Update();

		ProcessContinuesInputEvents();

		camera->EditorInfo();

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
			//objects[i]->Update(deltaTime);
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->EditorInfo();
		}

		imgui->Draw();

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
	/*for (int i = 0; i < 1000; i++)
{
	Object* bunny = new Object("Bunny.obj");
	bunny->transform.Position = vec3(RandomInRange(-3.5f, 3.5f), RandomInRange(-3.5f, 3.5f), RandomInRange(-3.5f, 3.5f));
	bunny->transform.Rotation = vec3(RandomInRange(-350.0f, 350.0f), RandomInRange(-350.0f, 350.0f), RandomInRange(-350.0f, 350.0f));
	objects.push_back(bunny);
}*/

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
		Camera::GetInstance()->CameraMovementSpeed = 500.0f;
	}
	
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		Camera::GetInstance()->CameraMovementSpeed = 2.5f;
	}
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