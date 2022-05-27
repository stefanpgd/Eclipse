#include "precomp.h"
#include "Renderer.h"

#include "Shader.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Object.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

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
	}
}

void Renderer::Update()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	for (int i = 0; i < 1000; i++)
	{
		Object* obj = new Object();
		obj->transform.Position = vec3(RandomInRange(-15.0f, 15.0f), RandomInRange(-15.0f, 15.0f), RandomInRange(-15.0f, 15.0f));
		obj->transform.Rotation = vec3(RandomInRange(-350.0f, 350.0f), RandomInRange(-350.0f, 350.0f), RandomInRange(-350.0f, 350.0f));
		float scale = RandomInRange(0.25f, 1.5f);
		obj->transform.Scale = vec3(scale);

		objects.push_back(obj);
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessContinuesInputEvents();
		camera->Update(deltaTime);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(deltaTime);
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello World");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::InputFloat3("Cam Position", &camera->CameraPosition[0]);
		ImGui::InputFloat3("Cam Front", &camera->CameraFront[0]);
		ImGui::InputFloat3("Cam Up", &camera->CameraUp[0]);
		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Renderer::CloseRenderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		FocusWindow = true;
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