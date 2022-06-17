#include "precomp.h"
#include "Renderer.h"
#include "Editor.h"
#include "Object.h"
#include "Camera.h"
#include <filesystem>
#include "DiffuseMaterial.h" // temp
#include "DefaultMaterial.h" // temp
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Model.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

Editor::Editor()
{
	GetAllModelFilePaths(modelFilePaths, modelFolderLoadPath, modelFolderLoadPath);
}

void Editor::DrawEditor(std::vector<Object*>& objects, std::vector<std::string>& consoleLog, float deltaTime)
{
	lastDeltaTime = deltaTime;
	
	SetWindowParameters();
	DrawMenubar();
	DrawSceneWindow(objects);

	if (objects.size() > 0)
	{
		if (objects[selectedObject] != nullptr)
		{
			DrawObjectDetails(objects[selectedObject]);
			DrawGizmos(objects[selectedObject]);
		}
		else
		{
			selectedObject = 0;
		}
	}

	DrawConsole(consoleLog);
	DrawStatistics();
}

void Editor::SetWindowParameters()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 880));
	ImGui::Begin("Scene Details", nullptr, ImGuiWindowFlags_NoMove);
	ImGui::End();

	if (showObjectDetails)
	{
		ImGui::SetNextWindowPos(ImVec2(1254, 20));
		ImGui::SetNextWindowSize(ImVec2(350, 475));
		ImGui::Begin("Object Details", nullptr, ImGuiWindowFlags_NoMove);
		ImGui::End();
	}

	if (showStatistics)
	{
		ImGui::SetNextWindowPos(ImVec2(285, 25));
		ImGui::SetNextWindowSize(ImVec2(100, 75));
		ImGui::SetNextWindowBgAlpha(0.45f); 
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::Begin("Statistics", nullptr, windowFlags);
		ImGui::End();
	}
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
		if (ImGui::Checkbox("Gizmos", &showGizmos)) {}
		if (ImGui::Checkbox("Console", &showConsole)) {}

		ImGui::Dummy(ImVec2(ScreenWidth - 755, 0));
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

	if (selectedObject > objects.size() - 1)
	{
		selectedObject = 0;
	}

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

		ImGui::DragFloat("Uniform Scale", &modelUniformScale, 1.0f, 0.0f, 0.0f, "%.0f");
		ImGui::InputInt("Material Index", &modelMaterialIndex);
		ImGui::InputText("Name", &newObjectName);
		ImGui::InputText("Model Dir", &modelFolderLoadPath[0], modelFolderLoadPath.size());

		if (ImGui::Button("Create Object"))
		{
			if (newObjectName.size() > 0)
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
				newObj->name = newObjectName.data();
				newObj->transform.Scale = vec3(modelUniformScale);
				objects.push_back(newObj);
			}
			else
			{
				Renderer::GetInstance()->ConsoleLog("Object needs to have a name/ID to be created", WarningLevel::Warning);
			}
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
			ImGui::PushID(objects[i]->ID);
			std::string objName = objects[i]->name.c_str();

			if (!objects[i]->isActive)
			{
				objName += " (Disabled)";
			}

			if (ImGui::Selectable(objName.c_str(), selectedObject == i))
			{
				selectedObject = i;
				placeholderName = "";
			}
			ImGui::PopID();
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

		// Object Name & Name update
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
			else
			{
				Renderer::GetInstance()->ConsoleLog("Can't update the Object name with an empty string", WarningLevel::Warning);
			}
		}
		ImGui::Separator();

		// Transform
		ImGui::PushFont(boldFont);
		ImGui::Text("Transform:");
		ImGui::PopFont();

		DrawVector3Edit("Position", object->transform.Position, 0.0f);
		ImGui::Spacing();
		DrawVector3Edit("Rotation", object->transform.Rotation, 0.0f);
		ImGui::Spacing();
		DrawVector3Edit("Scale", object->transform.Scale, 1.0f);
		ImGui::Spacing();

		ImGui::Separator();

		// Model Info
		ImGui::PushFont(boldFont);
		ImGui::Text("Model Info:");
		ImGui::PopFont();

		ImGui::Text("Model Name/Location:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), object->modelFileName.c_str());

		if (object->meshRenderer.usesModel)
		{
			std::string meshCount = std::to_string(object->meshRenderer.model->meshes.size());
			std::string textureCount = std::to_string(object->meshRenderer.model->texturesLoaded.size());

			ImGui::Text("Meshes:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), meshCount.c_str());

			ImGui::Text("Textures:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), textureCount.c_str());
			
			// Potential: Amount of verts?
		}
		ImGui::Separator();

		// Material Info
		ImGui::PushFont(boldFont);
		ImGui::Text("Material Info:");
		ImGui::PopFont();

		ImGui::InputInt("Material Index", &object->materialIndex);
		object->material->EditorInfo();

		// In the future allow to change materials at runtime...
		// this includes textures
		ImGui::Separator();

		// Option buttons ( delete, duplicate... enc. )
		ImGui::PushFont(boldFont);
		ImGui::Text("Functions:");
		ImGui::PopFont();

		if (ImGui::Button("Duplicate"))
		{
			object->Duplicate = true;
		}

		ImGui::SameLine(0, 3);
		if (ImGui::Button("Delete"))
		{
			object->Deleted = true;
		}
		ImGui::Separator();

		ImGui::End();
	}
}

void Editor::DrawGizmos(Object* object)
{
	if (showGizmos)
	{
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		if (ImGui::IsKeyPressed(87)) // W key
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69)) // E key
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // R Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		ImGuiIO& io = ImGui::GetIO();
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;
		static ImGuiWindowFlags gizmoWindowFlags = 0;

		Camera* camera = Camera::GetInstance();
		float* cameraView = value_ptr(camera->GetViewMatrix());
		float* cameraProjection = value_ptr(camera->GetProjectionMatrix());

		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		glm::mat4 tr(1.0f);
		float* modelMatrix = value_ptr(tr);
		vec3 pos = object->transform.Position;
		vec3 rot = object->transform.Rotation;
		vec3 scale = object->transform.Scale;
		ImGuizmo::RecomposeMatrixFromComponents(&pos[0], &rot[0], &scale[0], modelMatrix);
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, modelMatrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
		ImGuizmo::DecomposeMatrixToComponents(modelMatrix, &pos[0], &rot[0], &scale[0]);
		object->transform.Position = pos;
		object->transform.Rotation = rot;
		object->transform.Scale = scale;

		vec3 camPos = Camera::GetInstance()->CameraPosition;
		float camDistance = (camPos - object->transform.Position).length();

		float rightOffset = showObjectDetails ? 485 : 128;
		ImGuizmo::ViewManipulate(cameraView, 8.0f, ImVec2(viewManipulateRight - rightOffset, viewManipulateTop), ImVec2(128, 128), 0);
	}
}

void Editor::DrawConsole(std::vector<std::string>& consoleLog)
{
	// Maybe add an filter to only filter for logs/warnings/messages? Example does this as well.
	if (showConsole)
	{
		ImGui::Begin("Console");

		if(ImGui::Button("Clear Console"))
		{
			consoleLog.clear();
		}

		ImGui::Separator();

		ImGui::Checkbox("Auto Scroll", &autoScrollConsole);

		ImGui::Checkbox("Logs", &showLogs);
		ImGui::SameLine();
		ImGui::Checkbox("Warnings", &showWarnings);
		ImGui::SameLine();
		ImGui::Checkbox("Errors", &showErrors);
		ImGui::Separator();


		// Console is mainly based on the 'Example Console'
		// in the ImGui::ShowDemoWindow function

		const float footerHeightReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightReserve), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (size_t i = 0; i < consoleLog.size(); i++)
		{
			const char* message = consoleLog[i].c_str();

			ImVec4 textColor;
			if(strstr(message, "[Log]")) 
			{
				if (showLogs) { textColor = ImVec4(0.85f, 0.85f, 0.85f, 1.0f); }
				else { continue; }
			}
			else if (strstr(message, "[Warning]"))
			{
				if (showWarnings) { textColor = ImVec4(0.9f, 0.8f, 0.3f, 1.0f); }
				else { continue; }
			}
			else if (strstr(message, "[ERROR]"))
			{
				if (showErrors) { textColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); }
				else { continue; }
			}

			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			ImGui::TextUnformatted(message);
			ImGui::PopStyleColor();
		}

		if (autoScrollConsole && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::End();
	}
}

void Editor::DrawStatistics()
{
	if (showStatistics)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::Begin("Statistics", nullptr, windowFlags);

		ImGui::PushFont(boldFont);
		ImGui::Text("Statistics:");
		ImGui::PopFont();

		ImGui::Text("FPS:");
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), std::to_string(FPS).c_str());
		ImGui::PopFont();

		ImGui::Text("Draw Calls:");
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), std::to_string(Renderer::GetInstance()->DrawCalls).c_str());
		ImGui::PopFont();

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

void Editor::DrawVector3Edit(const std::string& name, glm::vec3& data, float resetValue)
{
	// Credits for this function go to Yan Chernikov, a.k.a. 'The Cherno'
	// source: https://youtu.be/oESRecjuLNY

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::PushID(name.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text(name.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 1));

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.81f, 0.1f, 0.15f, 1.0f));

	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		data.x = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &data.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		data.y = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &data.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		data.z = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &data.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}