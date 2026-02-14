#include <Renderer/VertexArray.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Core
{

std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::OpenGL:
		return std::make_shared<OpenGLVertexArray>();
	case RendererAPI::None:
		break;
	}

	assert(false && "Could not create Vertex Array since RendererAPI is none!");
	return nullptr;
}

}
