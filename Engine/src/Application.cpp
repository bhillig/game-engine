#include <Application.h>

#include <Renderer/RenderCommand.h>

#include <Platform/Windows/WindowsWindow.h>

#include <ImGui/CoreImGui.h>

#include <GLFW/glfw3.h>

#include <imgui.h>

#include <ranges>

namespace Core
{

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& appSpec)
	: m_appSpec(appSpec)
	, m_deltaTime(0.f)
	, m_lastTime(0.f)
	, m_minimized(false)
{
	assert(!s_instance && "Application is already instantiated!");
	s_instance = this;

	// Initialize logging immediately
	Log::Initialize();

	if (m_appSpec.WindowSpec.Title.empty())
	{
		m_appSpec.WindowSpec.Title = m_appSpec.Name;
	}

	m_appSpec.WindowSpec.Callback = [this](Event& event) {
		RaiseEvent(event);
	};

	m_window = std::make_unique<WindowsWindow>(m_appSpec.WindowSpec);

	// Construct Engine Subsystems
	m_assetManager = std::unique_ptr<AssetManager>(new AssetManager());
	m_renderer = std::unique_ptr<Renderer>(new Renderer());
}

bool Application::Init()
{
	if (!glfwInit())
	{
		LOG_CORE_CRITICAL("Failed to init glfw");
		return false;
	}

	if (!m_window->Initialize())
	{
		LOG_CORE_CRITICAL("Failed to create window");
		return false;
	}

	// Initialize Engine Subsystems

	if (!m_assetManager->Initialize())
	{
		LOG_CORE_CRITICAL("Failed to initialize asset manager");
		return false;
	}

	if (!m_renderer->Initialize())
	{
		LOG_CORE_CRITICAL("Failed to initialize renderer");
		return false;
	}

	CoreImGui::Initialize();

	LOG_CORE_INFO("Application '{}' Initialized", m_appSpec.Name);

	return true;
}

void Application::Run()
{
	m_running = true;
	m_lastTime = static_cast<float>(glfwGetTime());

	while (m_running)
	{
		// Calculate deltaTime
		const float currentTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		// Process events
		glfwPollEvents();

		if (!m_minimized)
		{
			// Update layers
			for (const auto& layer : m_layerStack)
			{
				layer->OnUpdate(m_deltaTime);
			}
		}

		// Render ImGui
		CoreImGui::Begin();

		const auto& caps = RenderCommand::GetCapabilities();

		// Display Renderer Information
		ImGui::Begin("Renderer");
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Renderer.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::Text("Frame Time: %.3fms", m_deltaTime * 1000);
		ImGui::End();

		for (const auto& layer : m_layerStack)
		{
			layer->OnImGuiRender();
		}
		CoreImGui::End();

		// Update AssetManager
		m_assetManager->Update();

		// Show updated buffer
		m_window->Update();
	}
}

const Window& Application::GetWindow() const
{
	assert(m_window && "Window not initialized!");
	return *m_window;
}

AssetManager& Application::GetAssetManager()
{
	assert(m_assetManager && "AssetManger not initialized!");
	return *m_assetManager;
}

Renderer& Application::GetRenderer()
{
	assert(m_renderer && "Renderer not initialized!");
	return *m_renderer;
}

void Application::RaiseEvent(Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowClosedEvent>(CORE_BIND_FN(Application::OnWindowClosed));
	dispatcher.Dispatch<WindowResizedEvent>(CORE_BIND_FN(Application::OnWindowResized));

	for (auto& layer : std::views::reverse(m_layerStack))
	{
		layer->OnEvent(event);
		if (event.Handled)
		{
			break;
		}
	}
}

bool Application::OnWindowClosed(WindowClosedEvent& event)
{
	m_running = false;
	return true;
}

bool Application::OnWindowResized(WindowResizedEvent& event)
{
	if (event.GetWidth() == 0 || event.GetHeight() == 0)
	{
		m_minimized = true;
		return false;
	}

	// TODO: Can be optimized. Should only update the viewport once when the window has stopped resizing.
	RenderCommand::SetViewport(0, 0, event.GetWidth(), event.GetHeight());

	m_minimized = false;
	return false;
}

Application::~Application()
{
	// Destroy resources in reverse order of creation
	m_layerStack.clear();

	CoreImGui::Shutdown();

	m_renderer.reset();
	m_assetManager.reset();
	m_window.reset();

	glfwTerminate();
	s_instance = nullptr;
}

} // namespace Core