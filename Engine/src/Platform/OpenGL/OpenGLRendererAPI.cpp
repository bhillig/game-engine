#include <Platform/OpenGL/OpenGLRendererAPI.h>

#include <glad/glad.h>

namespace Core
{

OpenGLRendererAPI::OpenGLRendererAPI()
{
}

OpenGLRendererAPI::~OpenGLRendererAPI()
{
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->GetElementBuffer()->Count()), GL_UNSIGNED_INT, nullptr);
}

}
