#include "Precomp.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "DiffuseMaterial.h"
#include "DefaultMaterial.h"
#include "SceneLoader.h"
#include <filesystem>

inline float Random01()
{
	return float(rand() / (RAND_MAX + 1.0));
}

inline float RandomInRange(float min, float max)
{
	return min + (max - min) * Random01();
}

void GetAllModelFilesInDirectory(std::vector<std::string>& files, std::string path, std::string originalPath)
{
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.is_directory())
		{
			GetAllModelFilesInDirectory(files, file.path().string(), originalPath);
		}

		std::string filePath = file.path().string();
		std::string fileType = filePath.substr(filePath.size() - 3);
		// path.substr(0, path.find_last_of('/'));

		if (fileType == "obj" || fileType == "fbx")
		{
			files.push_back(filePath.substr(originalPath.size() + 1));
		}
	}
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

		std::string path = "Assets/Models";
		GetAllModelFilesInDirectory(modelFilePaths, path, path);
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

		glViewport(0, 0, ScreenWidth, ScreenHeight);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		imgui->Update();

		ImGui::Begin("Editor");
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::Button("Scene Details"))
			{
				showSceneDetails = !showSceneDetails;
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::End();

		ImGui::ShowDemoWindow();

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
			objects[i]->EditorInfo(showSceneDetails);

			if (objects[i]->Duplicate)
			{
				Object* obj = new Object(*objects[i]);
				objects.push_back(obj);
				objects[i]->Duplicate = false;
			}

			if (objects[i]->Deleted)
			{
				Object* obj = objects[i];
				delete obj;
				objects[i] = nullptr;
			}
		}

		imgui->ActivateWindow("Create Object");
		
		if (ImGui::BeginCombo("Models", modelFilePaths[activeModelIndex].c_str()))
		{
			for (int i = 0; i < modelFilePaths.size(); i++)
			{
				bool isSelected = (modelFilePaths[activeModelIndex] == modelFilePaths[i]);
				if (ImGui::Selectable(modelFilePaths[i].c_str(), isSelected))
				{
					activeModelIndex = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
		
		ImGui::DragFloat("Uniform Scale", &uniformScale, 1.0f);
		ImGui::InputInt("Material Index", &selectedMaterial);
		ImGui::InputText("Name", &newObjectName[0], 50);

		if (ImGui::Button("Create Object"))
		{
			Material* mat;
			if (selectedMaterial == 0)
			{
				mat = new DiffuseMaterial();
			}
			else
			{
				mat = new DefaultMaterial();
			}

			Object* newObj = new Object(modelFilePaths[activeModelIndex], mat);
			newObj->materialIndex = selectedMaterial;
			newObj->name = newObjectName;
			newObj->transform.Scale = vec3(uniformScale);
			objects.push_back(newObj);
		}

		imgui->DisableWindow();

		imgui->Draw();

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			if (*it == nullptr)
			{
				objects.erase(it--);
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