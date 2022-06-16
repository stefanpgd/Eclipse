#include "precomp.h"
#include "Editor.h"
#include "Object.h"
#include "Camera.h"
#include <filesystem>
#include "DiffuseMaterial.h" // temp
#include "DefaultMaterial.h" // temp

Editor::Editor()
{
	GetAllModelFilePaths(modelFilePaths, modelFolderLoadPath, modelFolderLoadPath);
}

void Editor::DrawEditor(std::vector<Object*>& objects, float deltaTime)
{
	lastDeltaTime = deltaTime;

	SetWindowParameters();
	DrawMenubar();
	DrawSceneWindow(objects);
	DrawObjectDetails(objects[selectedObject]);
}

void Editor::SetWindowParameters()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::Begin("Scene Details", nullptr, ImGuiWindowFlags_NoMove);
	ImGui::End();
}

void Editor::DrawMenubar()
{
	frameCounterTimer += lastDeltaTime;
	if (frameCounterTimer >= frameCounterInterval)
	{
		frameCounterTimer = 0.0f;
		FPS = (int)(1.0f / lastDeltaTime);;
	}

	ImGui::Begin("Scene Details", nullptr, ImGuiWindowFlags_NoMove);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::Checkbox("Scene Objects", &showSceneObjects)) {}
		if (ImGui::Checkbox("Object Creation", &showObjectCreation)) {}
		if (ImGui::Checkbox("Camera Settings", &showCameraSettings)) {}
		if (ImGui::Checkbox("Object Details", &showObjectDetails)) {}
		if (ImGui::Checkbox("Statistics", &showStatistics)) {}

		ImGui::Dummy(ImVec2(ScreenWidth - 610, 0));
		ImGui::Text("FPS:");
		ImGui::TextColored(ImVec4(0.21f, 1.0f, 0.42f, 1.0f), std::to_string(FPS).c_str());
		ImGui::EndMainMenuBar();
	}

	ImGui::End();
}

void Editor::DrawSceneWindow(std::vector<Object*>& objects)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];
	ImGui::Begin("Scene Details", nullptr, ImGuiWindowFlags_NoMove);

	if (showObjectCreation)
	{
		ImGui::Separator();

		ImGui::PushFont(boldFont);
		ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.42f, 1.0f), "Object Creation");
		ImGui::PopFont();

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

		ImGui::DragFloat("Uniform Scale", &modelUniformScale, 1.0f);
		ImGui::InputInt("Material Index", &modelMaterialIndex);
		ImGui::InputText("Name", &newObjectName[0], 50);
		ImGui::InputText("Model Dir", &modelFolderLoadPath[0], modelFolderLoadPath.size());

		if (ImGui::Button("Create Object"))
		{
			Material* mat;
			if (modelMaterialIndex == 0)
			{
				mat = new DiffuseMaterial();
			}
			else
			{
				mat = new DefaultMaterial();
			}

			Object* newObj = new Object(modelFilePaths[activeModelIndex], mat);
			newObj->materialIndex = modelMaterialIndex;
			newObj->name = newObjectName;
			newObj->transform.Scale = vec3(modelUniformScale);
			objects.push_back(newObj);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reload Model Paths"))
		{
			modelFilePaths.clear();
			GetAllModelFilePaths(modelFilePaths, modelFolderLoadPath, modelFolderLoadPath);
		}

		ImGui::Separator();
	}

	if (showCameraSettings)
	{
		Camera* camera = Camera::GetInstance();

		ImGui::PushID(0);
		ImGui::Separator();

		ImGui::PushFont(boldFont);
		ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.275f, 1.0f), "Camera Settings");
		ImGui::PopFont();

		ImGui::DragFloat3("Position", &camera->CameraPosition[0], 10.0f);
		ImGui::DragFloat("Speed", &camera->CameraMovementSpeed, 0.1f);
		ImGui::DragFloat("Shift Speed", &camera->CameraMovementSprintSpeed, 5.0f);
		ImGui::DragFloat("Tilt Speed", &camera->CameraTiltSpeed, 0.1f);
		ImGui::DragFloat("Near", &camera->Near, 100.0f);
		ImGui::DragFloat("Far", &camera->Far, 100.0f);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 10));
		ImGui::PopID();
	}

	if (showSceneObjects)
	{
		ImGui::Separator();

		ImGui::PushFont(boldFont);
		ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.8f, 1.0f), "Objects in Scene:");
		ImGui::PopFont();

		for (size_t i = 0; i < objects.size(); i++)
		{
			std::string objName = objects[i]->name.c_str();

			if (!objects[i]->isActive)
			{
				objName += " (Disabled)";
			}

			if (ImGui::Selectable(objects[i]->name.c_str(), selectedObject == i))
			{
				selectedObject = i;
				placeholderName = "";
			}
		}
		ImGui::Separator();
	}

	ImGui::End();
}

void Editor::DrawObjectDetails(Object* object)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	if (showObjectDetails)
	{
		ImGui::Begin("Object Details");

		// Object Transform
		// Object Material
		// Mesh info ( model path, verts? meshes? )
		// Duplicate & Delete
		ImGui::Separator();
		ImGui::Text("Object Name:");
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::Text(object->name.c_str());
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::Checkbox("Is Active", &object->isActive)) {}

		ImGui::InputText("##", &placeholderName);
		ImGui::SameLine();
		if (ImGui::Button("Update Name"))
		{
			if (placeholderName.size() > 0)
			{
				object->name = placeholderName;
				placeholderName = "";
			}
		}
		ImGui::Separator();
		ImGui::PushFont(boldFont);
		ImGui::Text("Transform:");
		ImGui::PopFont();

		DrawVector3Edit("Position", object->transform.Position);
		DrawVector3Edit("Rotation", object->transform.Rotation);
		DrawVector3Edit("Scale", object->transform.Scale);

		ImGui::End();
	}
}

void Editor::GetAllModelFilePaths(std::vector<std::string>& files, std::string path, std::string originalPath)
{
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.is_directory())
		{
			GetAllModelFilePaths(files, file.path().string(), originalPath);
		}

		std::string filePath = file.path().string();
		std::string fileType = filePath.substr(filePath.size() - 3);

		// TODO: Replace subtr with proper one below:
		// path.substr(0, path.find_last_of('/'));
		if (fileType == "obj" || fileType == "fbx")
		{
			files.push_back(filePath.substr(originalPath.size() + 1));
		}
	}
}

void Editor::DrawVector3Edit(const std::string& name, glm::vec3& data)
{
}