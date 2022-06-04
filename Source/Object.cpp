#include "precomp.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "DefaultMaterial.h"
#include "DepthMaterial.h"
#include "DiffuseMaterial.h"
#include "Object.h"

Object::Object()
{
	ID = rand();
	material = new DepthMaterial();
	meshRenderer.Initialize();
	camera = Camera::GetInstance();
}

Object::Object(std::string fileName)
{
	ID = rand();
	material = new DepthMaterial();
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
	modelFileName = fileName;
}

Object::Object(std::string fileName, Material* material)
{
	ID = rand();
	this->material = material;
	meshRenderer.Initialize(fileName);
	camera = Camera::GetInstance();
	modelFileName = fileName;
}

Object::Object(ObjectSaveData& data)
{
	ID = rand();
	name = data.name;
	modelFileName = data.modelFileName;

	transform.SetTransformDataFromArray(data.transformInfo);

	if (data.modelFileName != "")
	{
		meshRenderer.Initialize(data.modelFileName);
	}
	else
	{
		meshRenderer.Initialize();
	}

	materialIndex = data.materialIndex;

	if (data.materialIndex == 0)
	{
		material = new DiffuseMaterial();
	}
	else
	{
		material = new DefaultMaterial();
	}

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
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	imgui->ActivateWindow("Editor");
	ImGui::PushID(ID);
	ImGui::InputText(" - Object Name", &name[0], 50);
	if (ImGui::CollapsingHeader(name.c_str(), true))
	{
		ImGui::Text("Transform");
		ImGui::Separator();
		ImGui::DragFloat3("Position", &transform.Position[0], 1.0f);
		ImGui::DragFloat3("Rotation", &transform.Rotation[0], 1.0f);
		ImGui::DragFloat3("Scale", &transform.Scale[0], 1.0f);

		ImGui::Separator();
		ImGui::Text("Model Name/Location:");
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1.0f), modelFileName.c_str());

		ImGui::InputInt("Material Index", &materialIndex);
	}
	ImGui::Separator();

	ImGui::Spacing();
	ImGui::PopID();
	imgui->DisableWindow();
}