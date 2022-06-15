#include "Precomp.h"

void ImguiHandler::Initialize(GLFWwindow* window)
{
	if (!isInitialized)
	{
		isInitialized = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		SetupStyle();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImGuizmo::SetOrthographic(false);
	}
}

void ImguiHandler::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImguiHandler::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiHandler::SetupStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 2;
	style.WindowRounding = 3;
	style.WindowTitleAlign = ImVec2(0.5, 0.5);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 0.56f, 0.00f, 0.50f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.64f, 0.64f, 0.64f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.70f, 0.25f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.90f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 0.49f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.67f, 0.67f, 0.67f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.42f, 0.00f, 0.64f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.60f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.36f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.54f, 0.23f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.77f, 0.77f, 0.77f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(1.00f, 0.38f, 0.12f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.59f, 0.20f, 0.00f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.70f, 0.56f, 0.92f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.35f);
}