#include <ImGui/ImGuiLayer.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Application.h>

namespace Core
{

void ImGuiLayer::OnAttach()
{
	const Window& window = Application::GetApp()->GetWindow();

	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multi-viewports
	ImGui::StyleColorsDark();

	// Setup backends
	ImGui_ImplGlfw_InitForOpenGL(Core::Application::GetApp()->GetWindow().GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::OnRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiLayer::~ImGuiLayer()
{
	// Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

}