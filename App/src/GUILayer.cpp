#include <GUILayer.h>

#include <Application.h>

#include <Events/InputEvents.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GUILayer::GUILayer()
	: m_hasControl(false)
{
}

void GUILayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& event) { return OnKeyPressed(event.GetKeyCode()); });
	dispatcher.Dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& event) { return m_hasControl; });
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& event) { return m_hasControl; });
}

bool GUILayer::OnKeyPressed(int key)
{
	if (key == KEY_ESCAPE)
	{
		ToggleControl();
		return false;
	}
	return m_hasControl;
}

void GUILayer::ToggleControl()
{
	const Core::Window& window = Core::Application::GetApp()->GetWindow();
	const int cursorMode = glfwGetInputMode(window.GetHandle(), GLFW_CURSOR);
	switch (cursorMode)
	{
	case GLFW_CURSOR_DISABLED:
		glfwSetInputMode(window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_hasControl = true;
		break;
	case GLFW_CURSOR_NORMAL:
		glfwSetInputMode(window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_hasControl = false;
		break;
	default:
		break;
	}
}

