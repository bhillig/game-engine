#include <Application.h>

#include <imgui.h>

#include <iostream>
#include <ranges>

namespace Core
{

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& appSpec)
	: m_appSpec(appSpec)
	, m_deltaTime(0.f)
	, m_lastTime(0.f)
{
	assert(!s_instance && "Application is already instantiated!");
	s_instance = this;

	if (m_appSpec.WindowSpec.Title.empty())
	{
		m_appSpec.WindowSpec.Title = m_appSpec.Name;
	}

	m_appSpec.WindowSpec.Callback = [this](Event& event) {
		RaiseEvent(event);
	};

	m_window = std::make_unique<Window>(m_appSpec.WindowSpec);

	// Construct Engine Subsystems
	m_renderer = std::unique_ptr<Renderer>(new Renderer());
}

bool Application::Init()
{
	// TODO: Add glfw error callback
	if (!glfwInit())
	{
		std::cerr << "Failed to init glfw!\n";
		return false;
	}

	if (!m_window->Create())
	{
		std::cerr << "Failed to create window!\n";
		return false;
	}

	// Initialize Engine Subsystems
	m_renderer->Initialize();

	return true;
}

void Application::Run()
{
	m_lastTime = static_cast<float>(glfwGetTime());

	while (!m_window->ShouldClose())
	{
		// Calculate deltaTime
		const float currentTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		// Process events
		glfwPollEvents();

		m_window->SetupGUIForFrame();

		// Update layers
		for (const auto& layer : m_layerStack)
		{
			layer->OnUpdate(m_deltaTime);
			layer->OnRender();
		}

		// Update AssetManager
		m_assetManager.Update();

		// Render GUI and show updated buffer
		m_window->RenderGUI();
		m_window->Show();
	}
}

const Window& Application::GetWindow() const
{
	assert(m_window && "Window not initialized!");
	return *m_window;
}

Renderer& Application::GetRenderer()
{
	assert(m_renderer && "Renderer not initialized!");
	return *m_renderer;
}

void Application::RaiseEvent(Event& event)
{
	for (auto& layer : std::views::reverse(m_layerStack))
	{
		layer->OnEvent(event);
		if (event.Handled)
		{
			break;
		}
	}
}

Application::~Application()
{
	glfwTerminate();
	s_instance = nullptr;
}

} // namespace Core