#include <Platform/OpenGL/OpenGLRendererAPI.h>

#include <glad/glad.h>

namespace
{

int DrawModeToOpenGLMode(Core::RendererAPI::DrawMode drawMode)
{
	switch (drawMode)
	{
		case Core::RendererAPI::DrawMode::Triangles:	return GL_TRIANGLES;
		case Core::RendererAPI::DrawMode::Lines:		return GL_LINES;
		default: 
			break;
	}
	return 0;
}

}

namespace Core
{

OpenGLRendererAPI::OpenGLRendererAPI()
{
}

OpenGLRendererAPI::~OpenGLRendererAPI()
{
}

void OpenGLRendererAPI::Init()
{
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto getString = [](GLenum name) -> std::string {
		const auto* str = glGetString(name);
		return str ? reinterpret_cast<const char*>(str) : "Unknown";
		};

	// Set Renderer Capabilities
	m_rendererCapabilities.Vendor = getString(GL_VENDOR);
	m_rendererCapabilities.Renderer = getString(GL_RENDERER);
	m_rendererCapabilities.Version = getString(GL_VERSION);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, DrawMode drawMode)
{
	glDrawElements(DrawModeToOpenGLMode(drawMode), static_cast<int>(vertexArray->GetElementBuffer()->Count()), GL_UNSIGNED_INT, nullptr);
}

}
