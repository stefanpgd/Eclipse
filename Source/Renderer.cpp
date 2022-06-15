#include "Precomp.h"
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

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
bool useWindow = false;

void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (editTransformDecomposition)
	{
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;

	//if (useWindow)
	//{
	//	ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
	//	ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
	//	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
	//	ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
	//	ImGuizmo::SetDrawlist();
	//	float windowWidth = (float)ImGui::GetWindowWidth();
	//	float windowHeight = (float)ImGui::GetWindowHeight();
	//	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	//	viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	//	viewManipulateTop = ImGui::GetWindowPos().y;
	//	ImGuiWindow* window = ImGui::GetCurrentWindow();
	//	gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
	//}
	//else
	//{
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//}

	mat4 id(1.0f);
	//ImGuizmo::DrawGrid(cameraView, cameraProjection, value_ptr(id), 100.f);
	//ImGuizmo::DrawCubes(cameraView, cameraProjection, matrix, 1);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	vec3 objPos = vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	vec3 camPos = Camera::GetInstance()->CameraPosition;

	float camDistance = (camPos - objPos).length();
	ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

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

		glm::mat4 tr(1.0f);
		tr = glm::scale(tr, vec3(1.0f, 1.0f, 1.0f));

		ImGui::Begin("Bob");

		useWindow = FocusWindow;

		ImGuizmo::SetID(0);
		glm::mat4 transform(1.0f);
		vec3 pos = objects[1]->transform.Position;
		vec3 rot = objects[1]->transform.Rotation;
		vec3 scale = objects[1]->transform.Scale;
		ImGuizmo::RecomposeMatrixFromComponents(&pos[0], &rot[0], &scale[0], value_ptr(transform));
		EditTransform(value_ptr(camera->GetViewMatrix()), value_ptr(camera->GetProjectionMatrix()), value_ptr(transform), true);
		ImGuizmo::DecomposeMatrixToComponents(value_ptr(transform), &pos[0], &rot[0], &scale[0]);
		objects[1]->transform.Position = pos;
		objects[1]->transform.Rotation = rot;
		objects[1]->transform.Scale = scale;
		ImGui::End();

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

		editor.DrawEditor(objects, deltaTime);

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