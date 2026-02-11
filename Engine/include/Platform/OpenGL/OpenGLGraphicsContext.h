#pragma once

#include <Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace Core
{

class OpenGLGraphicsContext final : public GraphicsContext
{
public:
	OpenGLGraphicsContext() = default;
	OpenGLGraphicsContext(GLFWwindow* handle);

	bool Initialize() override;

	void SwapBuffers() override;

protected:
	GLFWwindow* m_handle;
};

}
