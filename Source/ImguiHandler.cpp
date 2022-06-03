#include "Precomp.h"

void ImguiHandler::Initialize(GLFWwindow* window)
{
	if (!isInitialized)
	{
		isInitialized = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
}

void ImguiHandler::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImguiHandler::Draw()
{
	if (windowActivated)
	{
		windowActivated = false;
		DisableWindow();
		throw std::runtime_error("ERROR: 'ImguiHandler': When manually activating windows, make sure to disable them as well.\n");
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiHandler::CreateWindow(std::string title)
{
	ImGui::Begin(title.c_str(), nullptr);
	ImGui::End();
}

void ImguiHandler::CreateWindow(vec2 position, vec2 size, std::string title)
{
	ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	ImGui::Begin(title.c_str(), nullptr);
	ImGui::End();
}

void ImguiHandler::ActivateWindow(std::string windowName)
{
	windowActivated = true;
	ImGui::Begin(windowName.c_str(), nullptr);
}

void ImguiHandler::DisableWindow()
{
	windowActivated = false;
	ImGui::End();
}

void ImguiHandler::DrawText(std::string text)
{
	ImGui::Text(text.c_str());
}

void ImguiHandler::DrawText(std::string windowName, std::string text)
{
	ImGui::Begin(windowName.c_str(), nullptr);
	ImGui::Text(text.c_str());
	ImGui::End();
}

void ImguiHandler::SliderFloat(std::string name, float& value, float min, float max)
{
	ImGui::SliderFloat(name.c_str(), &value, min, max, "%.1f");
}

void ImguiHandler::SliderFloat(std::string windowName, std::string name, float& value, float min, float max)
{
	ImGui::Begin(windowName.c_str(), nullptr);
	ImGui::SliderFloat(name.c_str(), &value, min, max, "%.1f");
	ImGui::End();
}

void ImguiHandler::SliderVector3(std::string name, vec3& value, float min, float max)
{
	ImGui::SliderFloat3(name.c_str(), &value.x, min, max, "%.1f");
}

void ImguiHandler::SliderVector3(std::string windowName, std::string name, vec3& value, float min, float max)
{
	ImGui::Begin(windowName.c_str(), nullptr);
	ImGui::SliderFloat3(name.c_str(), &value.x, min, max, "%.1f");
	ImGui::End();
}

void ImguiHandler::InputVector3(std::string name, vec3& value)
{
	ImGui::InputFloat3(name.c_str(), &value.x, "%.1f");
}

void ImguiHandler::InputVector3(std::string windowName, std::string name, vec3& value)
{
	ImGui::Begin(windowName.c_str(), nullptr);
	ImGui::DragFloat3(name.c_str(), &value.x);
	ImGui::End();
}