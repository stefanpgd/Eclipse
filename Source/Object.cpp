#include "precomp.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "DefaultMaterial.h"
#include "DepthMaterial.h"
#include "Object.h"

Object::Object()
{
	material = new DepthMaterial();
	meshRenderer.Initialize();
	camera = Camera::GetInstance();
}

Object::Object(std::string fileName)
{
	material = new DepthMaterial();
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
}

Object::Object(std::string fileName, Material* material)
{
	this->material = material;
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
}

void Object::Update(float deltaTime)
{
	transform.Rotation.x += 25.0f * deltaTime;
	transform.Rotation.y += 15.0f * deltaTime;
	transform.Rotation.z += 5.0f * deltaTime;
}

void Object::Draw()
{
	if (isActive)
	{
		material->ActivateMaterial(transform.GetModelMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		meshRenderer.Draw(material->shader);
	}
}

void Object::DisplayInfo()
{
	ImguiHandler* imgui = ImguiHandler::GetInstance();
	imgui->CreateWindow(vec2(0, 0), vec2(300, 500), "Editor");
	imgui->ActivateWindow("Editor");

	if (ImGui::CollapsingHeader(name.c_str(), true))
	{
		ImGui::Text("Transform");
		ImGui::Separator();
		ImGui::DragFloat3("Position", &transform.Position[0], 1.0f);
		ImGui::DragFloat3("Rotation", &transform.Rotation[0], 1.0f);
		ImGui::DragFloat3("Scale", &transform.Scale[0], 1.0f);
	}

	imgui->DisableWindow();
}