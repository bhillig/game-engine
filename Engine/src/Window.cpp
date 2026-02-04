#include <Window.h>

#include <Events/InputEvents.h>
#include <Events/WindowEvents.h>

namespace Core
{

Window::Window(const WindowSpecification& windowSpec)
	: m_windowSpecification(windowSpec), m_handle(nullptr)
{
}

bool Window::Create()
{
	// Set window hints (OpenGL version and profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window 
	m_handle = glfwCreateWindow(m_windowSpecification.Width, m_windowSpecification.Height, m_windowSpecification.Title.c_str(), nullptr, nullptr);
	if (!m_handle)
	{
		LOG_CORE_ERROR("Failed to create GLFW window!");
		return false;
	}

	// Update context
	glfwMakeContextCurrent(m_handle);

	// Allows callbacks to access this application class
	glfwSetWindowUserPointer(m_handle, this);

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error_code, const char* description) {
		LOG_CORE_ERROR("GLFW Error {}: {}", error_code, description);
	});

	glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			KeyPressedEvent event(key, action == GLFW_REPEAT);
			self->m_windowSpecification.Callback(event);
		}
		else if (action == GLFW_RELEASE)
		{
			KeyReleasedEvent event(key);
			self->m_windowSpecification.Callback(event);
		}
	});

	glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		if (action == GLFW_PRESS)
		{
			MouseButtonPressedEvent event(button);
			self->m_windowSpecification.Callback(event);
		}
		else if (action == GLFW_RELEASE)
		{
			MouseButtonReleasedEvent event(button);
			self->m_windowSpecification.Callback(event);
		}
	});

	glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xPos, double yPos) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;
		
		MouseMovedEvent event(xPos, yPos);
		self->m_windowSpecification.Callback(event);
	});

	glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* window, int width, int height) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		self->m_windowSpecification.Width = width;
		self->m_windowSpecification.Height = height;
		glViewport(0, 0, width, height);

		WindowResizedEvent event(width, height);
		self->m_windowSpecification.Callback(event);
	});

	glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* window) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (!self) return;

		WindowClosedEvent event;
		self->m_windowSpecification.Callback(event);
	});

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_CORE_ERROR("Failed to initialize GLAD")
		return false;
	}

	// Capture mouse cursor
	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize viewport
	glViewport(0, 0, m_windowSpecification.Width, m_windowSpecification.Height);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Window::Show()
{
	// Swap front and back buffers
	glfwSwapBuffers(m_handle);
}

} // namespace Core
