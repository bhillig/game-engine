#include <Platform/OpenGL/OpenGLGraphicsContext.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{

OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* handle)
	: m_handle(handle)
{
}

bool OpenGLGraphicsContext::Initialize()
{
	// Set context
	glfwMakeContextCurrent(m_handle);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_CORE_ERROR("Failed to initialize GLAD");
		return false;
	}

	return true;
}

void OpenGLGraphicsContext::SwapBuffers()
{
	// Swap front and back buffers
	glfwSwapBuffers(m_handle);
}

}
