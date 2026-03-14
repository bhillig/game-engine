#include <Platform/Windows/WindowsWindow.h>

#include <Events/InputEvents.h>
#include <Events/WindowEvents.h>

#include <Platform/OpenGL/OpenGLGraphicsContext.h>

#include <GLFW/glfw3.h>

namespace Core
{

WindowsWindow::WindowsWindow(const WindowData& windowData)
	: m_windowData(windowData), m_window(nullptr)
{
}

bool WindowsWindow::Initialize()
{
	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window 
	m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		LOG_CORE_ERROR("Failed to create GLFW window!");
		return false;
	}

	// Update context
	m_graphicsContext = std::make_unique<OpenGLGraphicsContext>(m_window);
	if (!m_graphicsContext->Initialize())
	{
		LOG_CORE_ERROR("Failed to initialize graphics context!");
		return false;
	}

	// Allows callbacks to access this application class
	glfwSetWindowUserPointer(m_window, &m_windowData);

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error_code, const char* description) {
		LOG_CORE_ERROR("GLFW Error {}: {}", error_code, description);
		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			KeyPressedEvent event(key, action == GLFW_REPEAT);
			self->Callback(event);
		}
		else if (action == GLFW_RELEASE)
		{
			KeyReleasedEvent event(key);
			self->Callback(event);
		}
		});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		if (action == GLFW_PRESS)
		{
			MouseButtonPressedEvent event(button);
			self->Callback(event);
		}
		else if (action == GLFW_RELEASE)
		{
			MouseButtonReleasedEvent event(button);
			self->Callback(event);
		}
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			if (!self) return;

			MouseScrolledEvent event(xOffset, yOffset);
			self->Callback(event);
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		MouseMovedEvent event(xPos, yPos);
		self->Callback(event);
		});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		self->Width = width;
		self->Height = height;

		WindowResizedEvent event(width, height);
		self->Callback(event);
		});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		WindowData* self = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		WindowClosedEvent event;
		self->Callback(event);
		});

	// Capture mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	return true;
}

void WindowsWindow::Update()
{
	m_graphicsContext->SwapBuffers();
}

void* WindowsWindow::GetNativeHandle() const
{
	return m_window;
}

}
